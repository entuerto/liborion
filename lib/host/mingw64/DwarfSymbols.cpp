//
// DwarfSymbols.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <host/mingw64/DwarfSymbols.h>

#include <orion/io/MapFile.h>
#include <orion/Log.h>

#include <fmt/format.h>

namespace orion
{
namespace mingw64
{

static char unknown[] = { '?', '?', '\0' };

//--------------------------------------------------------------------------------------------------

struct AccessObject
{
   io::MapFile file_map;

   PIMAGE_NT_HEADERS nt_headers;
   PIMAGE_SECTION_HEADER sections;
   PIMAGE_SYMBOL symbol_table;
   PSTR string_table;
};

static std::unique_ptr<AccessObject> make_access_object(HANDLE file_handle)
{
   std::unique_ptr<AccessObject> access_object = std::make_unique<AccessObject>();

   uintptr_t handle = reinterpret_cast<uintptr_t>(file_handle);

   access_object->file_map = io::make_mapfile(handle, io::MapFlags::ReadOnly);

   auto* file_base = access_object->file_map.data();

   PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(file_base);

   access_object->nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(
      file_base + 
      dos_header->e_lfanew);

   access_object->sections = reinterpret_cast<PIMAGE_SECTION_HEADER>(
      (PBYTE)access_object->nt_headers + 
      sizeof(DWORD) + 
      sizeof(IMAGE_FILE_HEADER) +
      access_object->nt_headers->FileHeader.SizeOfOptionalHeader);

   access_object->symbol_table = reinterpret_cast<PIMAGE_SYMBOL>(
      file_base + 
      access_object->nt_headers->FileHeader.PointerToSymbolTable);

   access_object->string_table =
      (PSTR)&access_object->symbol_table[access_object->nt_headers->FileHeader.NumberOfSymbols];

   return access_object;
}

//--------------------------------------------------------------------------------------------------

namespace detail
{

static int get_section_info(void* obj,
                            Dwarf_Half section_index,
                            Dwarf_Obj_Access_Section* return_section,
                            int* error)
{
   AccessObject* pe_obj = static_cast<AccessObject*>(obj);

   return_section->addr = 0; // 0 is ok for non-ELF as per the comments in dwarf_opaque.h.

   if (section_index == 0)
   {
      // Non-elf object formats must honor elf convention that section index
      // is always empty.
      return_section->size = 0;
      return_section->name = "";
      return_section->link = 0;
      return DW_DLV_OK;
   }

   PIMAGE_SECTION_HEADER section = pe_obj->sections + section_index - 1;
   if (section == 0)
      return DW_DLV_ERROR;

   return_section->size = (section->Misc.VirtualSize < section->SizeOfRawData)
                             ? section->Misc.VirtualSize
                             : section->SizeOfRawData;

   return_section->name = reinterpret_cast<const char*>(section->Name);

   if (return_section->name[0] == '/')
   {
      return_section->name = &pe_obj->string_table[std::atoi(&return_section->name[1])];
   }
   
   return_section->link      = 0;
   return_section->entrysize = 0;

   return DW_DLV_OK;
}

static Dwarf_Endianness get_byte_order(void* obj)
{
   return DW_OBJECT_LSB; // always little-endian on Windows
}

static Dwarf_Small get_length_size(void* obj)
{
   AccessObject* pe_obj = static_cast<AccessObject*>(obj);

   PIMAGE_OPTIONAL_HEADER optional_header = &pe_obj->nt_headers->OptionalHeader;

   switch (optional_header->Magic)
   {
      case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
         return 4;
      case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
         return 8;
      default:
         return 0;
   }
}

static Dwarf_Small get_pointer_size(void* obj)
{
   return get_length_size(obj);
}

static Dwarf_Unsigned get_section_count(void* obj)
{
   AccessObject* pe_obj = static_cast<AccessObject*>(obj);

   PIMAGE_FILE_HEADER file_header = &pe_obj->nt_headers->FileHeader;

   return file_header->NumberOfSections + 1;
}

static int load_section(void* obj,
                        Dwarf_Half section_index,
                        Dwarf_Small** return_data,
                        int* error)
{
   AccessObject* pe_obj = static_cast<AccessObject*>(obj);

   if (section_index == 0)
      return DW_DLV_NO_ENTRY;

   PIMAGE_SECTION_HEADER section = pe_obj->sections + section_index - 1;
   *return_data                  = pe_obj->file_map.data() + section->PointerToRawData;
   return DW_DLV_OK;
}

// Initialize libdwarf using the "custom" interface that allows one to read the DWARF
// information contained in non-ELF files (PE/COFF in our case). 
static const Dwarf_Obj_Access_Methods pe_methods = {get_section_info,
                                                    get_byte_order,
                                                    get_length_size,
                                                    get_pointer_size,
                                                    get_section_count,
                                                    load_section,
                                                    nullptr};

} // namespace detail

//--------------------------------------------------------------------------------------------------

DwarfSymbols::DwarfSymbols() 
   : _access_obj()
   , _image()
{
}

DwarfSymbols::DwarfSymbols(HANDLE file_handle, const std::wstring& image)
   : _access_obj(make_access_object(file_handle))
   , _image(image)
{
}

DwarfSymbols::DwarfSymbols(DwarfSymbols&& rhs) noexcept
   : _access_obj(std::move(rhs._access_obj))
   , _image(std::move(rhs._image))
{
}

DwarfSymbols::~DwarfSymbols()
{
}

DwarfSymbols& DwarfSymbols::operator=(DwarfSymbols&& rhs) noexcept
{
   _access_obj = std::move(rhs._access_obj);
   _image      = std::move(rhs._image);

   return *this;
}

//--------------------------------------------------------------------------------------------------

struct Dwarf
{
   Dwarf_Debug dbg = nullptr;

   Dwarf_Arange* aranges = nullptr;
   Dwarf_Signed  aranges_count = 0;

   bool load_aranges()
   {
      Dwarf_Error error = 0;

      if (dwarf_get_aranges(dbg, &aranges, &aranges_count, &error) != DW_DLV_OK)
      {
         if (error) 
            log::error("DwarfSymbols: libdwarf error - ", dwarf_errmsg(error));
         return false;
      }

      return true;
   }

   void dealloc_aranges()
   {
      if (dbg == nullptr)
         return;

      for (Dwarf_Signed i = 0; i < aranges_count; ++i) 
      {
         dwarf_dealloc(dbg, aranges[i], DW_DLA_ARANGE);
      }
      dwarf_dealloc(dbg, aranges, DW_DLA_LIST);
   }
};

bool DwarfSymbols::find_symbol(Dwarf_Addr addr, SymbolInfo& info)
{
   Dwarf_Error error = 0;
   Dwarf dwarf;

   // Initialize the interface struct
   Dwarf_Obj_Access_Interface access{_access_obj.get(), &detail::pe_methods};

   if (dwarf_object_init(&access, 0, 0, &dwarf.dbg, &error) != DW_DLV_OK)
      return false;

   // Use the ".debug_aranges" DWARF section to pinpoint the CU (Compilation Unit) that
   // corresponds to the address we want to find information about. 
   if (not dwarf.load_aranges())
      return false;

   Dwarf_Arange arange;
   if (dwarf_get_arange(dwarf.aranges, dwarf.aranges_count, addr, &arange, &error) != DW_DLV_OK)
   {
      dwarf.dealloc_aranges();
      if (error) 
         log::error("DwarfSymbols: libdwarf error - ", dwarf_errmsg(error));
      return false;
   }

   Dwarf_Off cu_die_offset;
   // great, got a range that matches. let's find the CU it describes, and the DIE
   // (Debugging Information Entry) related to that CU. 
   if (dwarf_get_cu_die_offset(arange, &cu_die_offset, &error) != DW_DLV_OK)
   {     
      dwarf.dealloc_aranges();
      if (error) 
         log::error("DwarfSymbols: libdwarf error - ", dwarf_errmsg(error));
      return false;
   }

   Dwarf_Die cu_die;
   if (dwarf_offdie_b(dwarf.dbg, cu_die_offset, 1, &cu_die, &error) != DW_DLV_OK)
   {   
      dwarf.dealloc_aranges();
      if (error) 
         log::error("DwarfSymbols: libdwarf error - ", dwarf_errmsg(error));
      return false;
   }

   char* function_name = nullptr;

   search_func(dwarf.dbg, cu_die, addr, &function_name);
   if (function_name)
   {
      info.function_name = function_name;
      info.found         = true;
   }

   Dwarf_Line* linebuf;
   Dwarf_Signed linecount;
   if (dwarf_srclines(cu_die, &linebuf, &linecount, &error) == DW_DLV_OK)
   {
      Dwarf_Unsigned lineno, plineno;
      Dwarf_Addr lineaddr, plineaddr;
      char *file, *file0, *pfile;
      plineaddr = ~0ULL;
      plineno = lineno = 0;
      pfile = file = unknown;

      Dwarf_Signed i = 0;
      while (i < linecount)
      {
         if (dwarf_lineaddr(linebuf[i], &lineaddr, &error) != DW_DLV_OK)
         {
            log::error(fmt::format("DwarfSymbols: dwarf_lineaddr failed - %s\n", dwarf_errmsg(error)));
            break;
         }

         if (lineaddr == 0)
         {
            /* Per dwarfdump/print_lines.c, The SN Systems Linker generates
             * line records with addr=0, when dealing with linkonce symbols
             * and no stripping.  We need to skip records that do not have
             * ís_addr_set.
             */
            ++i;
            while (i < linecount)
            {
               Dwarf_Bool has_is_addr_set = FALSE;
               if (dwarf_line_is_addr_set(linebuf[i], &has_is_addr_set, &error) != DW_DLV_OK)
               {
                  log::error(fmt::format("DwarfSymbols: dwarf_line_is_addr_set failed - %s\n", dwarf_errmsg(error)));
                  has_is_addr_set = FALSE;
               }
               if (has_is_addr_set)
                  break;

               ++i;
            }
            continue;
         }

         if (addr > plineaddr and addr < lineaddr)
         {
            // Lines are past the address
            lineno = plineno;
            file   = pfile;
            break;
         }

         if (dwarf_lineno(linebuf[i], &lineno, &error) != DW_DLV_OK)
         {
            log::error(fmt::format("DwarfSymbols: dwarf_lineno failed - %s\n", dwarf_errmsg(error)));
            break;
         }

         if (dwarf_linesrc(linebuf[i], &file0, &error) != DW_DLV_OK)
         {
            log::error(fmt::format("DwarfSymbols: dwarf_linesrc failed - %s\n", dwarf_errmsg(error)));
         }
         else
         {
            file = file0;
         }

         if (addr == lineaddr)
            break; // Exact match

         plineaddr = lineaddr;
         plineno   = lineno;
         pfile     = file;
         ++i;
      }

      info.file_name = file;
      info.line      = lineno;

      dwarf_srclines_dealloc(dwarf.dbg, linebuf, linecount);
   }

   dwarf_dealloc(dwarf.dbg, cu_die, DW_DLA_DIE);

   if (error) 
   {
      log::error("DwarfSymbols: libdwarf error - ", dwarf_errmsg(error));
   }

   dwarf.dealloc_aranges();

   dwarf_object_finish(dwarf.dbg, &error);

   return true;
}

void DwarfSymbols::search_func(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Addr addr, char** rlt_func)
{
   Dwarf_Die spec_die;
   Dwarf_Die child_die;
   Dwarf_Die sibling_die;
   Dwarf_Half tag, return_form;
   Dwarf_Unsigned lopc, hipc;
   Dwarf_Off ref;
   Dwarf_Attribute sub_at, spec_at;
   
   do
   {
      if (*rlt_func != nullptr)
         return;

      Dwarf_Error error;

      if (dwarf_tag(die, &tag, &error) != DW_DLV_OK)
      {
         log::error(fmt::format("DwarfSymbols: dwarf_tag failed - %s", dwarf_errmsg(error)));
         goto cont_search;
      }

      if (tag == DW_TAG_subprogram)
      {
         enum Dwarf_Form_Class return_class;

         if (dwarf_lowpc(die, &lopc, &error) != DW_DLV_OK or
             dwarf_highpc_b(die, &hipc, &return_form, &return_class, &error) != DW_DLV_OK)
            goto cont_search;

         if (return_class == DW_FORM_CLASS_CONSTANT)
            hipc += lopc;

         if (addr < lopc || addr >= hipc)
            goto cont_search;

         /* Found it! */

         *rlt_func = unknown;

         int ret = dwarf_attr(die, DW_AT_name, &sub_at, &error);
         if (ret == DW_DLV_ERROR)
            return;

         if (ret == DW_DLV_OK)
         {
            char* func0;

            if (dwarf_formstring(sub_at, &func0, &error) != DW_DLV_OK)
               *rlt_func = unknown;
            else
               *rlt_func = func0;
            return;
         }

         /*
          * If DW_AT_name is not present, but DW_AT_specification is
          * present, then probably the actual name is in the DIE
          * referenced by DW_AT_specification.
          */
         if (dwarf_attr(die, DW_AT_specification, &spec_at, &error) != DW_DLV_OK)
            return;

         if (dwarf_global_formref(spec_at, &ref, &error) != DW_DLV_OK)
            return;

         if (dwarf_offdie(dbg, ref, &spec_die, &error) != DW_DLV_OK)
            return;

         if (dwarf_diename(spec_die, rlt_func, &error) != DW_DLV_OK)
            *rlt_func = unknown;

         return;
      }

   cont_search:

      /* Recurse into children. */
      int ret = dwarf_child(die, &child_die, &error);
      if (ret == DW_DLV_ERROR)
         log::error(fmt::format("DwarfSymbols: dwarf_child failed - %s", dwarf_errmsg(error)));
      else if (ret == DW_DLV_OK)
         search_func(dbg, child_die, addr, rlt_func);

      /* Advance to next sibling. */
      ret = dwarf_siblingof(dbg, die, &sibling_die, &error);
      if (ret != DW_DLV_OK)
      {
         if (ret == DW_DLV_ERROR)
            log::error(fmt::format("DwarfSymbols: dwarf_siblingof failed - %s", dwarf_errmsg(error)));
         break;
      }
      die = sibling_die;
   } while (true);
}

//--------------------------------------------------------------------------------------------------

DwarfSymbols make_dwarf_symbols(HANDLE file_handle, const std::wstring& image_name)
{
   return DwarfSymbols{file_handle, image_name};
}

} // namespace mingw64
} // namespace orion
