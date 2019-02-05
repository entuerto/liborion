//
// ModuleCache.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_DEBUG_MODULEPOOL_H
#define ORION_DEBUG_MODULEPOOL_H

#include <orion/Config.h>

#include <debug/Symbols.h>

#include <unordered_map>

namespace orion
{
namespace debug
{

//--------------------------------------------------------------------------------------------------

struct API_EXPORT Module
{
   uint64_t base;
   std::wstring file_name;

   Symbols symboles;

   Module(uint64_t b, const std::wstring& fname);

   bool find_symbol(uintptr_t address, LocationInfo& info);
};

//--------------------------------------------------------------------------------------------------

class API_EXPORT ModuleCache
{
public:
   ModuleCache();

   Module* find(uintptr_t address);

   static ModuleCache& instance();

private:
   Module* create(const std::wstring& file_name, uint64_t base);

   Module* lookup(uint64_t base);

   std::unordered_map<uint64_t, std::unique_ptr<Module>> _modules_by_base;
};

} // namespace debug
} // namespace orion

#endif // ORION_DEBUG_MODULEPOOL_H
