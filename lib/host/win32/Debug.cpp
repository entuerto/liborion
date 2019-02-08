//
// Stacktrace-Win32.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <host/win32/Debug.h>

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <Imagehlp.h>
#include <psapi.h>

namespace orion
{
namespace win32
{

static std::error_code capture_stacktrace_impl(std::vector<debug::Frame>& frames,
                                               int skip,
                                               int max_depth,
                                               CONTEXT& context)
{
   STACKFRAME64 stack_frame = {};

#if defined(_M_X64)
   stack_frame.AddrPC.Offset    = context.Rip;
   stack_frame.AddrStack.Offset = context.Rsp;
   stack_frame.AddrFrame.Offset = context.Rbp;
#else
   stack_frame.AddrPC.Offset    = context.Eip;
   stack_frame.AddrStack.Offset = context.Esp;
   stack_frame.AddrFrame.Offset = context.Ebp;
#endif

   stack_frame.AddrPC.Mode    = AddrModeFlat;
   stack_frame.AddrStack.Mode = AddrModeFlat;
   stack_frame.AddrFrame.Mode = AddrModeFlat;

   HANDLE process = GetCurrentProcess();
   HANDLE thread  = GetCurrentThread();

   // Set the symbol engine options
   DWORD sym_options = SymGetOptions();
   sym_options |= SYMOPT_DEFERRED_LOADS; // Do not load all symbols at once.
   sym_options |= SYMOPT_LOAD_LINES;     // Load line info
   // sym_options |= SYMOPT_EXACT_SYMBOLS;   // Do not load unmatched .PDB file.
   // sym_options |= SYMOPT_DEBUG;           // Symbol debug callback handler

   SymSetOptions(sym_options);

   // Initialize the symbol handler.
   SymInitialize(process, nullptr, TRUE);

   int i = 0;

   while (true)
   {
      if (StackWalk64(IMAGE_FILE_MACHINE_AMD64,
                      process,
                      thread,
                      &stack_frame,
                      &context,
                      nullptr,
                      SymFunctionTableAccess64,
                      SymGetModuleBase64,
                      nullptr)
          != TRUE)
      {
         break;
      }

      if (skip > 0)
      {
         --skip;
         continue;
      }

      if (stack_frame.AddrFrame.Offset == 0)
      {   
         break;
      }

      frames.emplace_back(debug::Frame{stack_frame.AddrPC.Offset});

      if (i >= max_depth)
      {
         break;
      }
      ++i;
   }

   SymCleanup(process);

   return {};
}

//--------------------------------------------------------------------------------------------------

std::error_code capture_stacktrace(std::vector<debug::Frame>& frames, int skip, int max_depth)
{
   CONTEXT context = {};

   ::RtlCaptureContext(&context);

   return capture_stacktrace_impl(frames, skip, max_depth, context);
}

} // namespace win32
} // namespace orion
