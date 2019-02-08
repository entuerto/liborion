//
// System-Win32.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/System.h>

#include <orion/Log.h>

#include <host/win32/COM.h>
#include <host/win32/Registry.h>
#include <host/win32/String.h>

#include <fmt/format.h>

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif
#include <lmcons.h> /* For UNLEN */
#include <process.h>
#include <windows.h>
#include <winternl.h>

#include <crtdbg.h>
#include <psapi.h>

#include <array>

namespace orion
{
namespace sys
{

//-------------------------------------------------------------------------------------------------

void get_loaded_modules(unsigned long process_id, ModuleList& modules)
{
   DWORD cbNeeded;

   // Get a list of all the modules in this process.
   HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);
   if (handle == nullptr)
      return;

   HMODULE module_handles[1024];

   if (EnumProcessModulesEx(
          handle, module_handles, sizeof(module_handles), &cbNeeded, LIST_MODULES_ALL))
   {
      int count = static_cast<int>(cbNeeded / sizeof(HMODULE));

      for (int i = 0; i < count; i++)
      {
         wchar_t module_name[MAX_PATH];

         // Get the full path to the module's file.
         if (GetModuleFileNameExW(handle, module_handles[i], module_name, sizeof(module_name)))
         {
            // Print the module name and handle value.
            modules.push_back(win32::wstring_to_utf8(module_name));
         }
      }
   }
   CloseHandle(handle);
}

//-------------------------------------------------------------------------------------------------

// The number of physical CPU cores (hyper-thread CPU count excluded)
static uint32_t get_cpu_count_phys()
{
   PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX buffer = nullptr;
   DWORD length                                    = 0;

   for (;;)
   {
      // GetLogicalProcessorInformationEx() is available from Windows 7
      // onward.
      if (GetLogicalProcessorInformationEx(RelationAll, buffer, &length) == TRUE)
         break;

      if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
      {
         if (buffer != nullptr)
            free(buffer);

         buffer = static_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(malloc(length));
         if (buffer == nullptr)
         {
            std::string error_message;

            win32::format_error_message(GetLastError(), error_message);
            log::error(error_message, DbgSrcLoc);
            return 0;
         }
         continue;
      }
      std::string error_message;

      win32::format_error_message(GetLastError(), error_message);
      log::error(error_message, DbgSrcLoc);
      return 0;
   }

   Span<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX> slpi = make_span(buffer, sizeof(buffer));

   uint32_t ncpus = 0;

   for (auto item : slpi)
   {
      if (item.Relationship == RelationProcessorCore)
         ++ncpus;
   }
   return ncpus;
}

//-------------------------------------------------------------------------------------------------

// Windows example values:
//
// struct CpuInfo
// {
//    std::string id;             Identifier:          Intel64 Family 6 Model 60 Stepping 3
//    std::string name;           ProcessorNameString: Intel(R) Core(TM) i7-4770 CPU @ 3.40GHz
//    std::string vendor;         VendorIdentifier:    GenuineIntel
//    uint32_t    speed;          ~MHz:                3392
//    uint32_t    cpu_count;      The number of logical, active CPUs.
//    uint32_t    cpu_count_phys; The number of physical CPU cores (hyper-thread CPU count excluded)
// };
//
CpuInfo get_cpu_info()
{
   const char* key_to_open = R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)";

   win32::Registry reg;

   std::error_code ec = reg.open_key(HKEY_LOCAL_MACHINE, key_to_open, KEY_READ);
   log::error_if(ec, DbgSrcLoc);

   std::wstring value;
   value.reserve(4096);

   //
   // Id
   //

   ec = reg.query_value("Identifier", value);
   log::error_if(ec, DbgSrcLoc);

   CpuInfo cpu;

   cpu.id = win32::wstring_to_utf8(value);

   //
   // Name
   //

   ec = reg.query_value("ProcessorNameString", value);
   log::error_if(ec, DbgSrcLoc);

   cpu.name = win32::wstring_to_utf8(value);

   //
   // Vendor
   //

   ec = reg.query_value("VendorIdentifier", value);
   log::error_if(ec, DbgSrcLoc);

   cpu.vendor = win32::wstring_to_utf8(value);

   //
   // Speed
   //

   ec = reg.query_value("~MHz", cpu.speed);
   log::error_if(ec, DbgSrcLoc);

   //
   // Number of logical, active CPUs
   //

   // GetActiveProcessorCount is available only on 64 bit versions
   // of Windows from Windows 7 onward.
   cpu.cpu_count = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);

   //
   // Number of physical CPU cores
   //

   // GetLogicalProcessorInformationEx() is available from Windows 7
   // onward.
   cpu.cpu_count_phys = get_cpu_count_phys();

   return cpu;
}

//-------------------------------------------------------------------------------------------------

CpuTimes get_cpu_times()
{
   CpuTimes cpu_times{};

   SYSTEM_INFO system_info;
   ZeroMemory(&system_info, sizeof(SYSTEM_INFO));

   GetSystemInfo(&system_info);

   uint32_t cpu_count = system_info.dwNumberOfProcessors;
   ULONG result_size  = 0u;
   DWORD sppi_size    = cpu_count * sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION);

   std::vector<SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION> sppi{cpu_count};

   NTSTATUS status = NtQuerySystemInformation(
      SystemProcessorPerformanceInformation, sppi.data(), sppi_size, &result_size);

   if (not NT_SUCCESS(status))
   {
      // err = RtlNtStatusToDosError(status);
      RtlNtStatusToDosError(status);
      return cpu_times;
   }

   for (const auto& item : sppi)
   {
      cpu_times.user += item.UserTime.QuadPart / 10000;
      cpu_times.nice += 0;
      cpu_times.sys += (item.KernelTime.QuadPart - item.IdleTime.QuadPart) / 10000;
      cpu_times.idle += item.IdleTime.QuadPart / 10000;
#if 0 // ndef __MINGW64__
      cpu_times.irq  += item.InterruptTime.QuadPart / 10000;
#else
      cpu_times.irq += item.Reserved1[0].QuadPart / 10000;
#endif
   }

   return cpu_times;
}

//-------------------------------------------------------------------------------------------------

std::string get_os_version()
{
   DWORD size = GetFileVersionInfoSizeW(L"C:\\windows\\System32\\version.dll", nullptr);
   if (size == 0)
   {
      return "Microsoft Windows (Unkown version)";
   }

   std::unique_ptr<BYTE> data(new BYTE[size]);

   if (GetFileVersionInfoW(L"C:\\windows\\System32\\version.dll", 0, size, data.get()) == 0)
   {
      return "Microsoft Windows (Unkown version)";
   }

   VS_FIXEDFILEINFO* file_info = nullptr;
   UINT len                    = 0;

   if (not VerQueryValueW(data.get(), TEXT("\\"), reinterpret_cast<LPVOID*>(&file_info), &len))
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

   return fmt::format("Microsoft Windows {0}.{1}.{2}.{3}", major, minor, hotfix, other);
}

//-------------------------------------------------------------------------------------------------

std::string get_host_name()
{
   wchar_t hostname[100];
   DWORD size         = sizeof(hostname);
   bool hostname_fail = GetComputerNameW(hostname, &size) == 0;

   return hostname_fail ? "localhost" : win32::wstring_to_utf8(hostname);
}

//-------------------------------------------------------------------------------------------------

std::string get_user_name()
{
   std::string user_name;

   uint32_t len = UNLEN + 1;
   wchar_t buffer[UNLEN + 1];

   if (GetUserNameW(buffer, reinterpret_cast<LPDWORD>(&len)) != 0)
   {
      user_name = win32::wstring_to_utf8(buffer);
   }
   return user_name;
}

//-------------------------------------------------------------------------------------------------

int get_process_id()
{
   return GetCurrentProcessId();
}

//-------------------------------------------------------------------------------------------------

std::string get_program_name()
{
   HANDLE ph = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, get_process_id());
   if (ph == nullptr)
   {
      return "";
   }

   wchar_t module_name[MAX_PATH];

   GetModuleFileNameExW(ph, nullptr, module_name, sizeof(module_name));

   CloseHandle(ph);

   return win32::wstring_to_utf8(module_name);
}

//-------------------------------------------------------------------------------------------------

void get_loadavg(double avg[3])
{
   avg[0] = avg[1] = avg[2] = 0;
}

//-------------------------------------------------------------------------------------------------

Size<si::Byte> get_free_memory()
{
   MEMORYSTATUSEX memory_status;
   memory_status.dwLength = sizeof(memory_status);

   if (GlobalMemoryStatusEx(&memory_status) == 0)
   {
      return {};
   }

   return Size<si::Byte>{static_cast<uintmax_t>(memory_status.ullAvailPhys)};
}

//-------------------------------------------------------------------------------------------------

Size<si::Byte> get_total_memory()
{
   MEMORYSTATUSEX memory_status;
   memory_status.dwLength = sizeof(memory_status);

   if (GlobalMemoryStatusEx(&memory_status) == 0)
   {
      return {};
   }

   return Size<si::Byte>{static_cast<uintmax_t>(memory_status.ullTotalPhys)};
}

} // namespace sys
} // namespace orion
