//
// System-Win32.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/System.h>

#include <orion/Log.h>

#include <host/win32/Registry.h>
#include <host/win32/String.h>

#include <fmt/format.h>

#include <lmcons.h> /* For UNLEN */
#include <process.h>
#include <windows.h>
#include <winternl.h>

#include <crtdbg.h>
#include <dbghelp.h>
#include <psapi.h>

#include <array>
#include <ostream>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

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
      for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
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
   PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX ptr    = nullptr;
   DWORD length                                    = 0;
   DWORD offset                                    = 0;
   DWORD ncpus                                     = 0;

   for(;;)
   {
      // GetLogicalProcessorInformationEx() is available from Windows 7
      // onward.
      if (GetLogicalProcessorInformationEx(RelationAll, buffer, &length) == TRUE)
         break;
      
      if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
      {
         if (buffer != nullptr)
            free(buffer);
            
         buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)malloc(length);
         if (buffer == nullptr)
         {
            std::string error_message;

            win32::format_error_message(GetLastError(), error_message);
            log::error(error_message, _src_loc);
            return 0;
         }
         continue;
      }
      std::string error_message;

      win32::format_error_message(GetLastError(), error_message);
      log::error(error_message, _src_loc);
      return 0;      
   }

   ptr = buffer;
   while (ptr->Size > 0 and (offset + ptr->Size) <= length) 
   {
      if (ptr->Relationship == RelationProcessorCore) 
         ncpus += 1;
        
      offset += ptr->Size;
      ptr = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*)(((char*)ptr) + ptr->Size);
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
   const char* key_to_open = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";

   win32::Registry reg;

   std::error_code ec = reg.open_key(HKEY_LOCAL_MACHINE, key_to_open, KEY_READ);
   if (ec)
      log::error(ec);

   std::wstring value;
   value.reserve(4096);

   //
   // Id
   //

   ec = reg.query_value("Identifier", value);
   if (ec)
      log::error(ec);

   CpuInfo cpu;

   cpu.id = win32::wstring_to_utf8(value);

   //
   // Name
   //

   ec = reg.query_value("ProcessorNameString", value);
   if (ec)
      log::error(ec);

   cpu.name = win32::wstring_to_utf8(value);

   //
   // Vendor
   //
   
   ec = reg.query_value("VendorIdentifier", value);
   if (ec)
      log::error(ec);

   cpu.vendor = win32::wstring_to_utf8(value);

   //
   // Speed
   //
   
   ec = reg.query_value("~MHz", cpu.speed);
   if (ec)
      log::error(ec);

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
      return cpu_times;
   }

   for (int i = 0; i < cpu_count; i++)
   {
      cpu_times.user += sppi[i].UserTime.QuadPart / 10000;
      cpu_times.nice += 0;
      cpu_times.sys  += (sppi[i].KernelTime.QuadPart - sppi[i].IdleTime.QuadPart) / 10000;
      cpu_times.idle += sppi[i].IdleTime.QuadPart / 10000;
#if 0 // ndef __MINGW64__
      cpu_times.irq  += sppi[i].InterruptTime.QuadPart / 10000;
#else
      cpu_times.irq += sppi[i].Reserved1[0].QuadPart / 10000;
#endif
   }
   free(sppi);

   return cpu_times;
}

//-------------------------------------------------------------------------------------------------

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

   return fmt::format("Microsoft Windows {0}.{1}.{2}.{3}", major, minor, hotfix, other);
}

//-------------------------------------------------------------------------------------------------

std::string get_host_name()
{
   wchar_t hostname[100];
   DWORD size         = sizeof(hostname);
   bool hostname_fail = not GetComputerNameW(hostname, &size);

   return hostname_fail ? "localhost" : win32::wstring_to_utf8(hostname);
}

//-------------------------------------------------------------------------------------------------

std::string get_user_name()
{
   std::string user_name;

   uint32_t len = UNLEN + 1;
   wchar_t buffer[UNLEN + 1];

   if (GetUserNameW(buffer, (LPDWORD)&len))
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
      return "";

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

Value<si::Kilobyte> get_free_memory()
{
   MEMORYSTATUSEX memory_status;
   memory_status.dwLength = sizeof(memory_status);

   if (not GlobalMemoryStatusEx(&memory_status))
   {
      return {};
   }

   return Value<si::Byte>{static_cast<uintmax_t>(memory_status.ullAvailPhys)};
}

//-------------------------------------------------------------------------------------------------

Value<si::Kilobyte> get_total_memory()
{
   MEMORYSTATUSEX memory_status;
   memory_status.dwLength = sizeof(memory_status);

   if (not GlobalMemoryStatusEx(&memory_status))
   {
      return {};
   }

   return  Value<si::Byte>{static_cast<uintmax_t>(memory_status.ullTotalPhys)};
}

//-------------------------------------------------------------------------------------------------

static void write_stack_trace_for_thread(std::ostream& os, 
                                         HANDLE hProcess,
                                         HANDLE hThread, 
                                         STACKFRAME64& StackFrame,
                                         CONTEXT* Context) 
{
   
   // Set the symbol engine options
   DWORD SymOptions = SymGetOptions();
   SymOptions |= SYMOPT_DEFERRED_LOADS;  // Do not load all symbols at once.
   SymOptions |= SYMOPT_LOAD_LINES;      // Load line info
   //SymOptions |= SYMOPT_EXACT_SYMBOLS;   // Do not load unmatched .PDB file.
   //SymOptions |= SYMOPT_DEBUG;               // Symbol debug callback handler

   SymSetOptions(SymOptions);

   // Initialize the symbol handler.
   SymInitialize(hProcess, NULL, TRUE);


   while (true) 
   {
      if (not StackWalk64(IMAGE_FILE_MACHINE_AMD64, hProcess, hThread, &StackFrame,
                          Context, 0, SymFunctionTableAccess64,
                          SymGetModuleBase64, 0)) 
      {
         break;
      }

      if (StackFrame.AddrFrame.Offset == 0)
         break;

      // Print the AddressPC in hexadecimal.
      DWORD64 AddressPC = StackFrame.AddrPC.Offset;

      os << fmt::format("{:#x}", AddressPC);

      // Verify the AddressPC belongs to a module in this process.
      if (not SymGetModuleBase64(hProcess, AddressPC)) 
      {
        os << " <Unknown module>\n";
        continue;
      }

      // Get module information
      IMAGEHLP_MODULE FoundModule;
      ZeroMemory(&FoundModule, sizeof(IMAGEHLP_MODULE));
      FoundModule.SizeOfStruct = sizeof(IMAGEHLP_MODULE);

      if (SymGetModuleInfo(hProcess, AddressPC, &FoundModule))
         os << fmt::format(" {:<15}", FoundModule.ModuleName);

      // Print the symbol name.
      char buffer[512];
      IMAGEHLP_SYMBOL64* symbol = reinterpret_cast<IMAGEHLP_SYMBOL64*>(buffer);
      memset(symbol, 0, sizeof(IMAGEHLP_SYMBOL64));
      symbol->SizeOfStruct  = sizeof(IMAGEHLP_SYMBOL64);
      symbol->MaxNameLength = 512 - sizeof(IMAGEHLP_SYMBOL64);

      DWORD64 dwDisp;
      if (not SymGetSymFromAddr64(hProcess, AddressPC, &dwDisp, symbol)) 
      {
         os << '\n';
         continue;
      }

      buffer[511] = 0;

      if (dwDisp > 0)
         os << fmt::format(" {0}() + {1:#x} byte(s)", symbol->Name, dwDisp);
      else
         os << fmt::format(" {}", symbol->Name);

      // Print the source file and line number information.
      IMAGEHLP_LINE64 line;
      ZeroMemory(&line, sizeof(IMAGEHLP_LINE));
      line.SizeOfStruct = sizeof(IMAGEHLP_LINE);
      DWORD dwLineDisp;

      if (SymGetLineFromAddr64(hProcess, AddressPC, &dwLineDisp, &line)) 
      {
        os << fmt::format(" {0}, line {1}", line.FileName, line.LineNumber);

        if (dwLineDisp > 0)
          os << fmt::format(" + {:#x} byte(s)", dwLineDisp);
      }
      os << '\n';
   }
}

//-------------------------------------------------------------------------------------------------

void write_stack_trace(std::ostream& os)
{
   STACKFRAME64 StackFrame = {};
   CONTEXT Context = {};
   
   ::RtlCaptureContext(&Context);

#if defined(_M_X64)
   StackFrame.AddrPC.Offset = Context.Rip;
   StackFrame.AddrStack.Offset = Context.Rsp;
   StackFrame.AddrFrame.Offset = Context.Rbp;
#else
   StackFrame.AddrPC.Offset = Context.Eip;
   StackFrame.AddrStack.Offset = Context.Esp;
   StackFrame.AddrFrame.Offset = Context.Ebp;
#endif

   StackFrame.AddrPC.Mode = AddrModeFlat;
   StackFrame.AddrStack.Mode = AddrModeFlat;
   StackFrame.AddrFrame.Mode = AddrModeFlat;

   write_stack_trace_for_thread(os, GetCurrentProcess(), GetCurrentThread(), StackFrame, &Context);
}

} // namespace sys
} // namespace orion
