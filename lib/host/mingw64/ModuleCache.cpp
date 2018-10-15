//
// ModulePool.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <host/mingw64/ModuleCache.h>

#include <orion/Log.h>

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <psapi.h>

#include <iostream>

namespace orion
{
namespace mingw64
{

static DWORD64 get_module_base(DWORD64 address)
{
   MEMORY_BASIC_INFORMATION mbi;

   VirtualQuery((void*)address, &mbi, sizeof(mbi));

   return (DWORD64)mbi.AllocationBase;
}

static std::wstring get_module_file_name(DWORD64 base)
{
   wchar_t data[MAX_PATH];

   auto handle = reinterpret_cast<HMODULE>(base);

   DWORD ret = GetModuleFileNameW(handle, data, MAX_PATH);
   if (ret == 0)
   {
      std::cout << "ModuleCache: could not determine module name.\n";
      return {};
   }

   return data;
}

//--------------------------------------------------------------------------------------------------
// Module

Module::Module(uint64_t b, const std::wstring& fname)
   : base(b)
   , file_name(fname)
   , symboles(debug::make_symbols(fname))
{
}

bool Module::find_symbol(uintptr_t address, debug::LocationInfo& info)
{
   try
   {
      return symboles.find_address(address, info);
   }
   catch (std::exception& e)
   {
      log::exception(e, _src_loc);
   }
   return false;
}

//--------------------------------------------------------------------------------------------------
// ModulePool

ModuleCache::ModuleCache() = default;

Module* ModuleCache::find(uintptr_t address)
{
   uint64_t base = get_module_base(address);
   if (base == 0)
      return nullptr;

   Module* module = lookup(base);
   if (module == nullptr)
      return nullptr;

   return module;
}

Module* ModuleCache::lookup(uint64_t base)
{
   auto search = _modules_by_base.find(base);
   if (search != _modules_by_base.end())
      return search->second.get();

   auto file_name = get_module_file_name(base);

   return create(file_name, base);
}

Module* ModuleCache::create(const std::wstring& file_name, uint64_t base)
{
   auto module = std::make_unique<Module>(base, file_name);

   auto it = _modules_by_base.emplace(base, std::move(module));

   return it.first->second.get();
}

ModuleCache& ModuleCache::instance()
{
   static ModuleCache cache;

   return cache;
}

} // namespace mingw64
} // namespace orion
