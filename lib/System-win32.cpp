// System-Win32.cpp
//
// Copyright 2013 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#include <orion/System.h>

#include <orion/StringUtils.h>

#include <boost/format.hpp>

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include <lmcons.h> /* For UNLEN */
#include <process.h>
#include <windows.h>

#if 0 // ndef __MINGW64__
#include <ddk/ntifs.h>

typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION {
LARGE_INTEGER IdleTime;
   LARGE_INTEGER KernelTime;
   LARGE_INTEGER UserTime;
   LARGE_INTEGER DpcTime;
   LARGE_INTEGER InterruptTime;
   ULONG InterruptCount;   
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;
#else
#include <winternl.h>
#endif

#include <psapi.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

namespace orion
{
namespace sys
{

//--------------------------------------------------------------------
void get_loaded_modules(unsigned long process_id, ModuleList& modules)
{
   DWORD cbNeeded;

   // Get a list of all the modules in this process.
   HANDLE process_handle =
      OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);
   if (process_handle == nullptr)
      return;

   HMODULE module_handles[1024];

   if (EnumProcessModulesEx(
          process_handle, module_handles, sizeof(module_handles), &cbNeeded, LIST_MODULES_ALL))
   {
      for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
      {
         wchar_t module_name[MAX_PATH];

         // Get the full path to the module's file.
         if (GetModuleFileNameExW(
                process_handle, module_handles[i], module_name, sizeof(module_name)))
         {
            // Print the module name and handle value.
            modules.push_back(wstring_to_utf8(module_name));
         }
      }
   }
   CloseHandle(process_handle);
}

std::string get_cpu_model()
{
   const uint32_t max_length = 4096;

   DWORD type   = REG_SZ;
   HKEY hKey    = nullptr;
   DWORD length = max_length;
   wchar_t value[max_length];

   ZeroMemory(value, max_length);

   const wchar_t* keyToOpen   = L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
   const wchar_t* valueToFind = L"ProcessorNameString";

   RegOpenKeyExW(HKEY_LOCAL_MACHINE, keyToOpen, 0, KEY_READ, &hKey);
   RegQueryValueExW(hKey, valueToFind, nullptr, &type, (LPBYTE)&value, &length);
   RegCloseKey(hKey);

   std::string cpuName = wstring_to_utf8(value);

   SYSTEM_INFO sysinfo;
   ZeroMemory(&sysinfo, sizeof(SYSTEM_INFO));
   GetSystemInfo(&sysinfo);

   const uint64_t cpuThreadCount = sysinfo.dwNumberOfProcessors;

   auto f = (cpuThreadCount == 1) ? boost::format("%s (%d thread)") % cpuName % cpuThreadCount :
                                    boost::format("%s (%d threads)") % cpuName % cpuThreadCount;

   return boost::str(f);
}

std::vector<CpuInfo> get_cpu_info()
{
   std::vector<CpuInfo> cpu_infos;

   SYSTEM_INFO system_info;
   ZeroMemory(&system_info, sizeof(SYSTEM_INFO));

   GetSystemInfo(&system_info);

   int32_t cpu_count = system_info.dwNumberOfProcessors;

   SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION* sppi = nullptr;
   ULONG result_size;

   DWORD sppi_size = cpu_count * sizeof(*sppi);
   sppi            = (SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION*)malloc(sppi_size);

   NTSTATUS status = NtQuerySystemInformation(
      SystemProcessorPerformanceInformation, sppi, sppi_size, &result_size);

   if (not NT_SUCCESS(status))
   {
      // err = RtlNtStatusToDosError(status);
      RtlNtStatusToDosError(status);
      free(sppi);
      return cpu_infos;
   }

   for (int32_t i = 0; i < cpu_count; i++)
   {
      WCHAR key_name[128];
      HKEY processor_key;

      int len = _snwprintf_s(key_name,
                             sizeof(key_name),
                             ARRAY_SIZE(key_name),
                             L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d",
                             i);

      DWORD ret = RegOpenKeyExW(HKEY_LOCAL_MACHINE, key_name, 0, KEY_QUERY_VALUE, &processor_key);

      if (ret != ERROR_SUCCESS)
      {
         // err = GetLastError();
         // free(sppi);
      }

      DWORD cpu_speed;
      DWORD cpu_speed_size = sizeof(cpu_speed);

      ret =
         RegQueryValueExW(processor_key, L"~MHz", NULL, NULL, (BYTE*)&cpu_speed, &cpu_speed_size);

      if (ret != ERROR_SUCCESS)
      {
         // err = GetLastError();
         // free(sppi);
         // RegCloseKey(processor_key);
      }

      WCHAR cpu_brand[256];
      DWORD cpu_brand_size = sizeof(cpu_brand);

      ret = RegQueryValueExW(
         processor_key, L"ProcessorNameString", NULL, NULL, (BYTE*)&cpu_brand, &cpu_brand_size);

      if (ret != ERROR_SUCCESS)
      {
         // err = GetLastError();
         // free(sppi);
         // RegCloseKey(processor_key);
      }

      RegCloseKey(processor_key);

      CpuTimes cpu_times;

      cpu_times.user = sppi[i].UserTime.QuadPart / 10000;
      cpu_times.nice = 0;
      cpu_times.sys  = (sppi[i].KernelTime.QuadPart - sppi[i].IdleTime.QuadPart) / 10000;
      cpu_times.idle = sppi[i].IdleTime.QuadPart / 10000;
#if 0 // ndef __MINGW64__
      cpu_times.irq  = sppi[i].InterruptTime.QuadPart / 10000;
#else
      cpu_times.irq = sppi[i].Reserved1[0].QuadPart / 10000;
#endif

      len = WideCharToMultiByte(
         CP_UTF8, 0, cpu_brand, cpu_brand_size / sizeof(WCHAR), NULL, 0, NULL, NULL);

      if (len == 0)
      {
         // err = GetLastError();
         // free(sppi);
      }

      char* model = new char[len];

      len = WideCharToMultiByte(
         CP_UTF8, 0, cpu_brand, cpu_brand_size / sizeof(WCHAR), model, len, NULL, NULL);

      if (len == 0)
      {
         // err = GetLastError();
         // free(sppi);
      }

      cpu_infos.push_back(CpuInfo{model, cpu_speed, cpu_times});

      delete[] model;
   }

   free(sppi);

   return cpu_infos;
}

std::string get_os_version()
{
   DWORD size = GetFileVersionInfoSizeW(L"C:\\windows\\System32\\version.dll", nullptr);
   if (size == 0)
      return "Microsoft Windows (Unkown version)";

   std::unique_ptr<BYTE> data(new BYTE[size]);

   if (not GetFileVersionInfoW(L"C:\\windows\\System32\\version.dll", 0, size, data.get()))
   {
      return "Microsoft Windows (Unkown version)";
   }

   VS_FIXEDFILEINFO* file_info = nullptr;
   UINT len                    = 0;

   if (not VerQueryValue(data.get(), TEXT("\\"), (LPVOID*)&file_info, &len))
   {
      return "Microsoft Windows (Unkown version)";
   }

   if (len == 0)
   {
      return "Microsoft Windows (Unkown version)";
   }

   int major  = HIWORD(file_info->dwProductVersionMS);
   int minor  = LOWORD(file_info->dwProductVersionMS);
   int hotfix = HIWORD(file_info->dwProductVersionLS);
   int other  = LOWORD(file_info->dwProductVersionLS);

   auto f = boost::format("Microsoft Windows %d.%d.%d.%d") % major % minor % hotfix % other;

   return boost::str(f);
}

std::string get_host_name()
{
   wchar_t hostname[100];
   DWORD size         = sizeof(hostname);
   bool hostname_fail = not GetComputerNameW(hostname, &size);

   return hostname_fail ? "localhost" : wstring_to_utf8(hostname);
}

std::string get_user_name()
{
   std::string user_name;

   uint32_t len = UNLEN + 1;
   wchar_t buffer[UNLEN + 1];

   if (GetUserNameW(buffer, (LPDWORD)&len))
   {
      user_name = wstring_to_utf8(buffer);
   }
   return user_name;
}

int get_process_id()
{
   return GetCurrentProcessId();
}

std::string get_program_name()
{
   HANDLE process_handle =
      OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, get_process_id());
   if (process_handle == nullptr)
      return "";

   wchar_t module_name[MAX_PATH];

   GetModuleFileNameExW(process_handle, nullptr, module_name, sizeof(module_name));

   CloseHandle(process_handle);

   return wstring_to_utf8(module_name);
}

void get_loadavg(double avg[3])
{
   avg[0] = avg[1] = avg[2] = 0;
}

uint64_t get_free_memory()
{
   MEMORYSTATUSEX memory_status;
   memory_status.dwLength = sizeof(memory_status);

   if (not GlobalMemoryStatusEx(&memory_status))
   {
      return -1;
   }

   return static_cast<uint64_t>(memory_status.ullAvailPhys);
}

uint64_t get_total_memory()
{
   MEMORYSTATUSEX memory_status;
   memory_status.dwLength = sizeof(memory_status);

   if (not GlobalMemoryStatusEx(&memory_status))
   {
      return -1;
   }

   return static_cast<uint64_t>(memory_status.ullTotalPhys);
}

} // namespace sys
} // namespace orion
