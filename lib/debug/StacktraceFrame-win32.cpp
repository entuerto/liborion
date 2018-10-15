//
// StacktraceFrame-win32.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/debug/Stacktrace.h>

#include <host/win32/Dbgeng.h>

namespace orion
{
namespace debug
{

static win32::ComPtr<::IDebugSymbols>& get_debug_inst() 
{
   // [class.mfct]: A static local variable or local type in a member function always refers to the
   // same entity, whether or not the member function is inline.
   static thread_local win32::ComInitializer com;
   static thread_local win32::ComPtr<::IDebugSymbols> dbg_symboles;

   if (not dbg_symboles)
   {
      win32::dbgeng::try_init(dbg_symboles);
   }

   return dbg_symboles;
}

std::string Frame::module_name() const
{
   win32::ComPtr<::IDebugSymbols>& dbg_symboles = get_debug_inst();
   return win32::dbgeng::module_name(dbg_symboles, reinterpret_cast<void*>(_address));
}

std::string Frame::function_name() const
{
   win32::ComPtr<::IDebugSymbols>& dbg_symboles = get_debug_inst();
   return win32::dbgeng::func_name(dbg_symboles, reinterpret_cast<void*>(_address));
}

std::string Frame::source_file() const
{
   win32::ComPtr<::IDebugSymbols>& dbg_symboles = get_debug_inst();
   return win32::dbgeng::source_file(dbg_symboles, reinterpret_cast<void*>(_address));
}

uint64_t Frame::source_line() const
{
   win32::ComPtr<::IDebugSymbols>& dbg_symboles = get_debug_inst();
   return win32::dbgeng::source_line(dbg_symboles, reinterpret_cast<void*>(_address));
}

} // namespace debug
} // namespace orion
