//
// Symbols.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_DEBUG_SYMBOLS_H
#define ORION_DEBUG_SYMBOLS_H

#include <orion/Config.h>

#include <memory>
#include <string>

namespace orion
{
namespace debug
{
struct SymbolsImpl;

//--------------------------------------------------------------------------------------------------

struct LocationInfo
{
   std::string file_name;
   std::string function_name;
   uint32_t line = 0;
};

//--------------------------------------------------------------------------------------------------

class Symbols
{
public:
   explicit Symbols(const std::wstring& file_name);
   Symbols(uintptr_t file_handle, const std::wstring& name);
   ~Symbols();

   // No copy
   Symbols(const Symbols&) = delete;
   Symbols& operator=(const Symbols&) = delete;

   // Move
   Symbols(Symbols&& rhs) noexcept;
   Symbols& operator=(Symbols&& rhs) noexcept;

   bool find_address(uintptr_t address, LocationInfo& location_info) const;

   void dump() const;

private:
   const SymbolsImpl* impl() const { return _impl.get(); }
   SymbolsImpl* impl() { return _impl.get(); }

   std::unique_ptr<SymbolsImpl> _impl;
};

Symbols make_symbols(const std::wstring& file_name);

Symbols make_symbols(uintptr_t file_handle, const std::wstring& image_name);

} // namespace debug
} // namespace orion

#endif // ORION_DEBUG_SYMBOLS_H
