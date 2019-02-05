//
// Dbgeng.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_HOST_WIN32_DBGENG_H
#define ORION_HOST_WIN32_DBGENG_H

#include <orion/Config.h>

#include <host/win32/COM.h>

#include <fmt/format.h>

#include <array>

#include <dbgeng.h>
#include <guiddef.h>
#include <objbase.h>

#ifdef __CRT_UUID_DECL // for __MINGW32__
__CRT_UUID_DECL(IDebugClient,0x27fe5639,0x8407,0x4f47,0x83,0x64,0xee,0x11,0x8f,0xb0,0x8a,0xc8);
__CRT_UUID_DECL(IDebugControl,0x5182e668,0x105e,0x416e,0xad,0x92,0x24,0xef,0x80,0x04,0x24,0xba);
__CRT_UUID_DECL(IDebugSymbols,0x8c31e98c,0x983a,0x48a5,0x90,0x16,0x6f,0xe5,0xd6,0x67,0xa9,0x50);
__CRT_UUID_DECL(IDebugDataSpaces3,0x23f79d6c,0x8aaf,0x4f7c,0xa6,0x07,0x99,0x95,0xf5,0x40,0x7e,0x63);
#endif

namespace orion
{
namespace win32
{
//--------------------------------------------------------------------------------------------------
// dbgeng functions
//
namespace dbgeng
{

static void try_init(win32::ComPtr<::IDebugSymbols>& dbg_symboles)
{
   win32::ComPtr<::IDebugClient> dbg_client;
   if (S_OK !=
       ::DebugCreate(__uuidof(IDebugClient), reinterpret_cast<void**>(dbg_client.address_of())))
      return;

   win32::ComPtr<::IDebugControl> dbg_control;
   if (S_OK != dbg_client.as(dbg_control))
      return;

   if (S_OK !=
       dbg_client->AttachProcess(0,
                                 ::GetCurrentProcessId(),
                                 DEBUG_ATTACH_NONINVASIVE | DEBUG_ATTACH_NONINVASIVE_NO_SUSPEND))
      return;

   if (S_OK != dbg_control->WaitForEvent(DEBUG_WAIT_DEFAULT, INFINITE))
      return;

   // No cheking: QueryInterface sets the output parameter to NULL in case of error.
   dbg_client.as(dbg_symboles);
}

static std::string module_name(win32::ComPtr<::IDebugSymbols>& dbg_symboles, const void* addr)
{
   const ULONG64 offset = reinterpret_cast<ULONG64>(addr);

   ULONG64 base = 0;

   HRESULT res =
      dbg_symboles->GetModuleByOffset(offset, 0 /*StartIndex*/, nullptr /*Index*/, &base);

   if (res != S_OK)
      return "";

   char data[MAX_PATH];

   auto handle = reinterpret_cast<HMODULE>(base);

   DWORD ret = GetModuleFileNameA(handle, data, MAX_PATH);
   if (ret == 0)
      return "";

   return data;
}

static std::string func_name(win32::ComPtr<::IDebugSymbols>& dbg_symboles, const void* addr)
{

   const ULONG64 offset = reinterpret_cast<ULONG64>(addr);

   constexpr static int buffer_size   = 256;
   std::array<char, buffer_size> name = {
      0,
   };
   ULONG size = 0;

   HRESULT res = dbg_symboles->GetNameByOffset(offset, name.data(), name.size(), &size, nullptr);

   std::string result;
   if (res != S_OK and size != 0)
   {
      result.resize(size);
      res = dbg_symboles->GetNameByOffset(
         offset, &result[0], static_cast<ULONG>(result.size()), &size, nullptr);
   }
   else if (res == S_OK)
   {
      result = name.data();
   }

   if (res != S_OK)
   {
      result.clear();
      return result;
   }

   const std::size_t delimiter = result.find_first_of('!');

   // If 'delimiter' is equal to 'std::string::npos' then we have only module name.
   if (delimiter == std::string::npos) 
   {
      result.clear();
      return result;
   }

   return result.substr(delimiter + 1);
}

static std::string source_file(win32::ComPtr<::IDebugSymbols>& dbg_symboles, const void* addr)
{
   const ULONG64 offset = reinterpret_cast<ULONG64>(addr);

   const int buffer_size              = 256;
   std::array<char, buffer_size> name = {
      0,
   };
   ULONG size = 0;

   HRESULT res =
      dbg_symboles->GetLineByOffset(offset, nullptr, name.data(), name.size(), &size, nullptr);

   std::string result;
   if (res != S_OK and size != 0)
   {
      result.resize(size);
      res = dbg_symboles->GetLineByOffset(
         offset, nullptr, &result[0], static_cast<ULONG>(result.size()), &size, nullptr);
   }
   else if (res == S_OK)
   {
      result = name.data();
   }

   if (res != S_OK)
   {
      result.clear();
   }
   return result;
}

static std::size_t source_line(win32::ComPtr<::IDebugSymbols>& dbg_symboles, const void* addr)
{
   const ULONG64 offset = reinterpret_cast<ULONG64>(addr);

   ULONG line_number = 0;

   HRESULT res = dbg_symboles->GetLineByOffset(offset, &line_number, nullptr, 0, nullptr, nullptr);

   return (res == S_OK) ? line_number : 0;
}

} // namespace dbgeng
} // namespace win32
} // namespace orion

#endif // ORION_HOST_WIN32_DBGENG_H
