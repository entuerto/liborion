//
// COFF.cpp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <debug/dw/COFF.h>

#include <iostream>

namespace orion
{
namespace debug
{
namespace dw
{
//--------------------------------------------------------------------------------------------------
//
void Object::dump() const
{
   auto section_count = nt_headers->FileHeader.NumberOfSections;

   for (int i = 0; i < section_count; ++i)
   {
      std::cout << "Section: " << i << " name: ";

      for (int j = 0; j < IMAGE_SIZEOF_SHORT_NAME; ++j)
      {
         std::cout << sections[i].Name[j];
      }
      std::cout << "\n";
   }
}

//--------------------------------------------------------------------------------------------------

int get_section_info(void* obj,
                     Dwarf_Half section_index,
                     Dwarf_Obj_Access_Section* return_section,
                     int* /*error*/)
{
   auto pe_obj = static_cast<Object*>(obj);

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
   if (section == nullptr)
      return DW_DLV_ERROR;

   return_section->size = (section->Misc.VirtualSize < section->SizeOfRawData)
                             ? section->Misc.VirtualSize
                             : section->SizeOfRawData;

   return_section->name = reinterpret_cast<const char*>(section->Name);

   if (return_section->name[0] == '/')
   {
      return_section->name = &(pe_obj->string_table[std::atoi(&(return_section->name[1]))]);
   }

   return_section->link      = 0;
   return_section->entrysize = 0;

   return DW_DLV_OK;
}

Dwarf_Endianness get_byte_order(void* /*obj*/)
{
   return DW_OBJECT_LSB; // always little-endian on Windows
}

Dwarf_Small get_length_size(void* obj)
{
   auto pe_obj = static_cast<Object*>(obj);

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

Dwarf_Small get_pointer_size(void* obj)
{
   return get_length_size(obj);
}

Dwarf_Unsigned get_section_count(void* obj)
{
   auto pe_obj = static_cast<Object*>(obj);

   PIMAGE_FILE_HEADER file_header = &pe_obj->nt_headers->FileHeader;

   return file_header->NumberOfSections + 1;
}

int load_section(void* obj, Dwarf_Half section_index, Dwarf_Small** return_data, int* /*error*/)
{
   auto pe_obj = static_cast<Object*>(obj);

   if (section_index == 0)
      return DW_DLV_NO_ENTRY;

   PIMAGE_SECTION_HEADER section = pe_obj->sections + section_index - 1;
   *return_data                  = pe_obj->file_map.data() + section->PointerToRawData;
   return DW_DLV_OK;
}

//--------------------------------------------------------------------------------------------------

std::unique_ptr<Object> make_object(const std::wstring& file_name)
{
   std::unique_ptr<Object> object = std::make_unique<Object>();

   object->file_map = io::make_mapfile(file_name, io::MapFlags::ReadOnly);

   auto* file_base = object->file_map.data();

   auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(file_base);

   object->nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(file_base + dos_header->e_lfanew);

   object->sections = reinterpret_cast<PIMAGE_SECTION_HEADER>(
      reinterpret_cast<PBYTE>(object->nt_headers) + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) +
      object->nt_headers->FileHeader.SizeOfOptionalHeader);

   object->symbol_table = reinterpret_cast<PIMAGE_SYMBOL>(
      file_base + object->nt_headers->FileHeader.PointerToSymbolTable);

   object->string_table =
      reinterpret_cast<PSTR>(&object->symbol_table[object->nt_headers->FileHeader.NumberOfSymbols]);

   return object;
}

std::unique_ptr<Object> make_object(uintptr_t file_handle)
{
   std::unique_ptr<Object> object = std::make_unique<Object>();

   object->file_map = io::make_mapfile(file_handle, io::MapFlags::ReadOnly);

   auto* file_base = object->file_map.data();

   auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(file_base);

   object->nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(file_base + dos_header->e_lfanew);

   object->sections = reinterpret_cast<PIMAGE_SECTION_HEADER>(
      reinterpret_cast<PBYTE>(object->nt_headers) + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) +
      object->nt_headers->FileHeader.SizeOfOptionalHeader);

   object->symbol_table = reinterpret_cast<PIMAGE_SYMBOL>(
      file_base + object->nt_headers->FileHeader.PointerToSymbolTable);

   object->string_table =
      reinterpret_cast<PSTR>(&object->symbol_table[object->nt_headers->FileHeader.NumberOfSymbols]);

   return object;
}

} // namespace dw
} // namespace debug
} // namespace orion