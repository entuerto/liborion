//
// ModulePool.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_HOST_MINGW64_MODULEPOOL_H
#define ORION_HOST_MINGW64_MODULEPOOL_H

#include <orion/Orion-Stddefs.h>

#include <orion/io/MapFile.h>

#include <host/mingw64/DwarfSymbols.h>
#include <host/win32/String.h>

#include <Imagehlp.h>
#include <dbghelp.h>

#include <unordered_map>

namespace orion
{
namespace mingw64
{

struct Module
{
   Module();

   DwarfSymbols symboles;
   
   DWORD64 base;
   std::wstring loaded_image_name;

   io::MapFile file_map;

   DWORD64 image_base_vma;

   // Headers
   PIMAGE_NT_HEADERS nt_headers() const;

   PIMAGE_OPTIONAL_HEADER optional_header() const;

   PIMAGE_OPTIONAL_HEADER32 optional_header32() const;
   PIMAGE_OPTIONAL_HEADER64 optional_header64() const;

   PIMAGE_SECTION_HEADER sections() const;
   PIMAGE_SYMBOL symbol_table() const;

   bool find_symbol(DWORD64 address, SymbolInfo& info);
};

class ModulePool
{
public:
   ModulePool();

   Module* find(HANDLE process, DWORD64 address);
   
   static ModulePool& instance();

private:
   Module* create(HANDLE process, HANDLE file_handle, DWORD64 base);

   Module* create(HANDLE process, const std::wstring& image_name, DWORD64 base);

   Module* lookup(HANDLE process, DWORD64 base);

   std::unordered_map<DWORD64, std::unique_ptr<Module>> _modules_by_process;
};


} // namespace mingw64
} // namespace orion

#endif // ORION_HOST_MINGW64_MODULEPOOL_H
