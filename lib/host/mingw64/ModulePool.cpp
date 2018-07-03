//
// ModulePool.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <host/mingw64/ModulePool.h>

//#include <orion/Log.h>

#include <fmt/format.h>

#include <psapi.h>

#include <cxxabi.h>
#include <iostream>

namespace orion
{
namespace mingw64
{

//
// The GetModuleBase function retrieves the base address of the module that
// contains the specified address.
//
static DWORD64 get_module_base(HANDLE process, DWORD64 address)
{
   if (process == GetCurrentProcess())
   {
      HMODULE module = nullptr;
      BOOL ret       = GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                       GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                                    (LPCSTR)(UINT_PTR)address,
                                    &module);
      if (ret)
      {
         return (DWORD64)(UINT_PTR)module;
      }
   }

   MEMORY_BASIC_INFORMATION buffer;
   if (VirtualQueryEx(process, (LPCVOID)(UINT_PTR)address, &buffer, sizeof buffer) != 0)
   {
      return (DWORD64)(UINT_PTR)buffer.AllocationBase;
   }

   return SymGetModuleBase64(process, address);
}

static std::wstring get_module_file_name(HANDLE process, DWORD64 base)
{
   wchar_t data[MAX_PATH];

   // SymGetModuleInfo64 is not reliable for this, as explained in
   // https://msdn.microsoft.com/en-us/library/windows/desktop/ms681336.aspx
   //
   DWORD ret = GetModuleFileNameExW(process, (HMODULE)(UINT_PTR)base, data, MAX_PATH);
   if (ret == 0)
   {
      std::cout << "ModulePool: could not determine module name.\n";
      return {};
   }

   return data;
}

//--------------------------------------------------------------------------------------------------
// Module

Module::Module() 
   : base(0)
   , loaded_image_name()
   , file_map()
   , image_base_vma(0)
{
}

PIMAGE_NT_HEADERS Module::nt_headers() const
{
   uint8_t* file_base = const_cast<uint8_t*>(file_map.data());

   PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(file_base);

   return reinterpret_cast<PIMAGE_NT_HEADERS>(file_base + dos_header->e_lfanew);
}

PIMAGE_OPTIONAL_HEADER Module::optional_header() const
{
   return &(nt_headers()->OptionalHeader);
}

PIMAGE_OPTIONAL_HEADER32 Module::optional_header32() const
{
   return (optional_header()->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
             ? reinterpret_cast<PIMAGE_OPTIONAL_HEADER32>(optional_header())
             : nullptr;
}

PIMAGE_OPTIONAL_HEADER64 Module::optional_header64() const
{
   return (optional_header()->Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
             ? reinterpret_cast<PIMAGE_OPTIONAL_HEADER64>(optional_header())
             : nullptr;
}

PIMAGE_SECTION_HEADER Module::sections() const
{
   return reinterpret_cast<PIMAGE_SECTION_HEADER>(
      (PBYTE)nt_headers() +
      sizeof(DWORD) +
      sizeof(IMAGE_FILE_HEADER) +
      nt_headers()->FileHeader.SizeOfOptionalHeader);
}

PIMAGE_SYMBOL Module::symbol_table() const
{
   uint8_t* file_base = const_cast<uint8_t*>(file_map.data());

   return reinterpret_cast<PIMAGE_SYMBOL>(
      file_base +
      nt_headers()->FileHeader.PointerToSymbolTable);
}

bool Module::find_symbol(DWORD64 address, SymbolInfo& info)
{
   return symboles.find_symbol(address, info);
}

//--------------------------------------------------------------------------------------------------
// ModulePool

ModulePool::ModulePool() {}

Module* ModulePool::find(HANDLE process, DWORD64 address)
{
   DWORD64 base = get_module_base(process, address);
   if (base == 0)
      return nullptr;

   Module* module = lookup(process, base);
   if (module == nullptr)
      return nullptr;

   return module;
}

Module* ModulePool::lookup(HANDLE process, DWORD64 base)
{
   auto search = _modules_by_process.find(base);
   if (search != _modules_by_process.end()) 
      return search->second.get();

   auto file_name = get_module_file_name(process, base);

   return create(process, file_name, base);
}

Module* ModulePool::create(HANDLE process, HANDLE file_handle, DWORD64 base)
{
   // auto module = std::make_unique<Module>();
    
   return nullptr;
}

Module* ModulePool::create(HANDLE process, const std::wstring& image_name, DWORD64 base)
{
   auto module = std::make_unique<Module>();

   module->base              = base;
   module->loaded_image_name = image_name;

   module->file_map = io::make_mapfile(module->loaded_image_name, io::MapFlags::ReadOnly);

   HANDLE file_handle = reinterpret_cast<HANDLE>(module->file_map.file_handle());

   module->symboles = make_dwarf_symbols(file_handle, module->loaded_image_name);

   auto it = _modules_by_process.emplace(base, std::move(module));

   return it.first->second.get();
}

ModulePool& ModulePool::instance()
{
   static ModulePool pool;

   return pool;
}

} // namespace mingw64
} // namespace orion
