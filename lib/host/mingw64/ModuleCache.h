//
// ModulePool.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_HOST_MINGW64_MODULEPOOL_H
#define ORION_HOST_MINGW64_MODULEPOOL_H

#include <orion/Config.h>

#include <Debug/Symbols.h>

#include <unordered_map>

namespace orion
{
namespace mingw64
{

//--------------------------------------------------------------------------------------------------

struct Module
{
   uint64_t base;
   std::wstring file_name;

   debug::Symbols symboles;

   Module(uint64_t b, const std::wstring& fname);

   bool find_symbol(uintptr_t address, debug::LocationInfo& info);
};

//--------------------------------------------------------------------------------------------------

class ModuleCache
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

} // namespace mingw64
} // namespace orion

#endif // ORION_HOST_MINGW64_MODULEPOOL_H
