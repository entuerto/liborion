//
// DwarfSymbols.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <debug/Symbols.h>

#include <debug/dw/COFF.h>
#include <debug/dw/DwarfWrapper.h>

#include <iostream>

namespace orion
{
namespace debug
{

//--------------------------------------------------------------------------------------------------

struct SymbolsImpl
{
   std::unique_ptr<dw::Object> object;

   Dwarf_Obj_Access_Interface obj_access;

   std::wstring file_name;

   std::unique_ptr<dw::Debug> dbg;

   explicit SymbolsImpl(const std::wstring& fname)
      : object(dw::make_object(fname))
      , obj_access()
      , file_name(fname)
      , dbg()
   {
      obj_access.object  = object.get();
      obj_access.methods = &dw::methods;

      dbg = std::make_unique<dw::Debug>(obj_access);
   }

   SymbolsImpl(uintptr_t file_handle, const std::wstring& fname)
      : object(dw::make_object(file_handle))
      , obj_access()
      , file_name(fname)
      , dbg()
   {
      obj_access.object  = object.get();
      obj_access.methods = &dw::methods;

      dbg = std::make_unique<dw::Debug>(obj_access);
   }
};

//--------------------------------------------------------------------------------------------------

Symbols::Symbols(const std::wstring& file_name)
   : _impl(std::make_unique<SymbolsImpl>(file_name))
{
}

Symbols::Symbols(uintptr_t file_handle, const std::wstring& name)
   : _impl(std::make_unique<SymbolsImpl>(file_handle, name))
{
}

Symbols::Symbols(Symbols&& rhs) noexcept
   : _impl(std::move(rhs._impl))
{
}

Symbols::~Symbols() = default;

Symbols& Symbols::operator=(Symbols&& rhs) noexcept
{
   _impl = std::move(rhs._impl);

   return *this;
}

void Symbols::dump() const
{
   std::wcout << "Symbols -> " << impl()->file_name << "\n";
   impl()->dbg->address_ranges().dump();
}

bool Symbols::find_address(uintptr_t address, LocationInfo& location_info) const
{
   if (impl()->dbg->empty())
      return false;

   // Use the ".debug_aranges" DWARF section to pinpoint the CU (Compilation Unit) that
   // corresponds to the address we want to find information about.
   const dw::AddressRangeTable& arange_table = impl()->dbg->address_ranges();

   const dw::AddressRange& arange = arange_table.find(address);

   const dw::CompileUnit& cu =
      arange.empty() ? impl()->dbg->compile_units().find(address) : arange.compile_unit();

   if (cu.empty())
      return false;

   // great, got a range that matches. let's find the CU it describes, and the DIE
   // (Debugging Information Entry) related to that CU.
   const dw::Die& die = cu.die();

   if (die.empty())
      return false;

   // Inside this CU, find the exact statement (DWARF calls it a "line") that
   // corresponds to the address we want to find information about.
   dw::LineTable line_table = die.line_table();

   dw::LineEntry entry = line_table.find(address);

   auto& src_files = die.source_files();

   location_info.file_name     = src_files[entry.src_file_id];
   location_info.function_name = die.find_function(address);
   location_info.line          = entry.lineno;
   return true;
}

//--------------------------------------------------------------------------------------------------

Symbols make_symbols(const std::wstring& file_name)
{
   return Symbols{file_name};
}

Symbols make_symbols(uintptr_t file_handle, const std::wstring& image_name)
{
   return Symbols{file_handle, image_name};
}

} // namespace debug
} // namespace orion
