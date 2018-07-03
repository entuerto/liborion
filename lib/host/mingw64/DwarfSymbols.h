//
// DwarfSymbols.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_HOST_MINGW64_DWARFSYMBOLS_H
#define ORION_HOST_MINGW64_DWARFSYMBOLS_H

#include <orion/Orion-Stddefs.h>

#include <dwarf/dwarf.h>
#include <dwarf/libdwarf.h>

#include <memory>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace orion
{
namespace mingw64
{
struct AccessObject;

struct SymbolInfo
{
   std::string file_name;
   std::string function_name;
   uint32_t line = 0;
   bool found = false;
};

class DwarfSymbols
{
public:
   DwarfSymbols();
   DwarfSymbols(HANDLE file_handle, const std::wstring& image);
   ~DwarfSymbols();

   // No copy
   DwarfSymbols(const DwarfSymbols&) = delete;
   DwarfSymbols& operator=(const DwarfSymbols&) = delete;

   // Move
   DwarfSymbols(DwarfSymbols&& rhs) noexcept;
   DwarfSymbols& operator=(DwarfSymbols&& rhs) noexcept;

   bool find_symbol(Dwarf_Addr addr, SymbolInfo& info);

private:
   void search_func(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Addr addr, char** rlt_func);

   std::unique_ptr<AccessObject> _access_obj;
   std::wstring _image;
};

DwarfSymbols make_dwarf_symbols(HANDLE file_handle, const std::wstring& image_name);

} // namespace mingw64
} // namespace orion

#endif // ORION_HOST_MINGW64_DWARFSYMBOLS_H
