//
// COFF.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_DEBUG_DW_COFF_H
#define ORION_DEBUG_DW_COFF_H

#include <orion/io/MapFile.h>

#include <dwarf.h>
#include <libdwarf.h>

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <memory>

namespace orion
{
namespace debug
{
namespace dw
{
//--------------------------------------------------------------------------------------------------
//
struct Object
{
   io::MapFile file_map;

   PIMAGE_NT_HEADERS nt_headers;
   PIMAGE_SECTION_HEADER sections;
   PIMAGE_SYMBOL symbol_table;
   PSTR string_table;

   void dump() const;
};

//--------------------------------------------------------------------------------------------------

int get_section_info(void* obj,
                     Dwarf_Half section_index,
                     Dwarf_Obj_Access_Section* return_section,
                     int* error);

Dwarf_Endianness get_byte_order(void* obj);

Dwarf_Small get_length_size(void* obj);

Dwarf_Small get_pointer_size(void* obj);

Dwarf_Unsigned get_section_count(void* obj);

int load_section(void* obj, Dwarf_Half section_index, Dwarf_Small** return_data, int* error);

// Initialize libdwarf using the "custom" interface that allows one to read the DWARF
// information contained in non-ELF files (PE/COFF in our case).
static const Dwarf_Obj_Access_Methods methods = {get_section_info,
                                                 get_byte_order,
                                                 get_length_size,
                                                 get_pointer_size,
                                                 get_section_count,
                                                 load_section,
                                                 nullptr};

//--------------------------------------------------------------------------------------------------

std::unique_ptr<Object> make_object(const std::wstring& file_name);

std::unique_ptr<Object> make_object(uintptr_t file_handle);

} // namespace dw
} // namespace debug
} // namespace orion

#endif // ORION_DEBUG_DW_COFF_H
