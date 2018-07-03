//
// Debug.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_HOST_MINGW64_DEBUG_H
#define ORION_HOST_MINGW64_DEBUG_H

#include <orion/Orion-Stddefs.h>

#include <orion/Log.h>
#include <orion/debug/Stacktrace.h>

#include <host/mingw64/ModulePool.h>
#include <host/win32/String.h>

#include <Imagehlp.h>
#include <dbghelp.h>
#include <psapi.h>

namespace orion
{
namespace mingw64
{

static bool find_symbol(HANDLE process, DWORD64 address, SymbolInfo& info)
{
   ModulePool& pool = ModulePool::instance();

   Module* module = pool.find(process, address);

   if (module == nullptr)
   {
      log::error("Module not found.");
      return false;
   }

   return module->find_symbol(address, info);
}

static std::error_code capture_stacktrace(std::vector<debug::Frame>& frames,
                                          int skip,
                                          int max_depth)
{
   STACKFRAME64 StackFrame = {};
   CONTEXT Context         = {};

   ::RtlCaptureContext(&Context);

#if defined(_M_X64)
   StackFrame.AddrPC.Offset    = Context.Rip;
   StackFrame.AddrStack.Offset = Context.Rsp;
   StackFrame.AddrFrame.Offset = Context.Rbp;
#else
   StackFrame.AddrPC.Offset    = Context.Eip;
   StackFrame.AddrStack.Offset = Context.Esp;
   StackFrame.AddrFrame.Offset = Context.Ebp;
#endif

   StackFrame.AddrPC.Mode    = AddrModeFlat;
   StackFrame.AddrStack.Mode = AddrModeFlat;
   StackFrame.AddrFrame.Mode = AddrModeFlat;

   HANDLE process = GetCurrentProcess();
   HANDLE thread  = GetCurrentThread();

   // Set the symbol engine options
   DWORD SymOptions = SymGetOptions();
   SymOptions |= SYMOPT_DEFERRED_LOADS; // Do not load all symbols at once.
   SymOptions |= SYMOPT_LOAD_LINES;     // Load line info
   // SymOptions |= SYMOPT_EXACT_SYMBOLS;   // Do not load unmatched .PDB file.
   // SymOptions |= SYMOPT_DEBUG;           // Symbol debug callback handler

   SymSetOptions(SymOptions);

   // Initialize the symbol handler.
   SymInitialize(process, nullptr, TRUE);

   SymbolInfo info;

   while (true)
   {
      if (not StackWalk64(IMAGE_FILE_MACHINE_AMD64,
                          process,
                          thread,
                          &StackFrame,
                          &Context,
                          0,
                          SymFunctionTableAccess64,
                          SymGetModuleBase64,
                          0))
      {
         break;
      }

      if (StackFrame.AddrFrame.Offset == 0)
         break;

      DWORD64 address = StackFrame.AddrPC.Offset;

      if (find_symbol(process, address, info))
      {
         frames.emplace_back(
            debug::Frame{address, "", info.function_name, info.file_name, info.line});
      }
   }

   return std::error_code();
}

} // namespace mingw64
} // namespace orion

#endif // ORION_HOST_MINGW64_DEBUG_H
