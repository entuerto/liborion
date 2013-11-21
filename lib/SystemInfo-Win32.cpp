// Win32-utils.cpp
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

#include <orion/SystemInfo.h>

#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <lmcons.h>   /* For UNLEN */
#include <process.h>

#include <psapi.h>

namespace orion
{
namespace systeminfo
{

void get_loaded_modules(unsigned long process_id, ModuleList& modules)
{
   DWORD cbNeeded;

   // Get a list of all the modules in this process.
   HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION |
                                       PROCESS_VM_READ,
                                       FALSE, process_id);
   if (NULL == process_handle)
       return;

   HMODULE module_handles[1024];

   if (EnumProcessModules(process_handle, module_handles, sizeof(module_handles), &cbNeeded)) {
      for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
         TCHAR module_name[MAX_PATH];

         // Get the full path to the module's file.
         if (GetModuleFileNameEx(process_handle,
                                 module_handles[i],
                                 module_name, sizeof(module_name) / sizeof(TCHAR))) {
            // Print the module name and handle value.
            modules.push_back(module_name);
         }
      }
   }
   CloseHandle(process_handle);
}

std::string get_cpu_info()
{
   DWORD type = REG_SZ;
   HKEY hKey = nullptr;
   const uint32_t max_length = 4096;

   DWORD length = max_length;
   char  value[max_length];
   ZeroMemory(value, max_length);

   const char* keyToOpen = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
   const char* valueToFind = "ProcessorNameString";

   RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyToOpen, 0, KEY_READ, &hKey);
   RegQueryValueEx(hKey, valueToFind, NULL, &type, (LPBYTE)&value, &length);
   RegCloseKey(hKey);

   std::string cpuName = value;

   SYSTEM_INFO sysinfo;
   ZeroMemory(&sysinfo, sizeof(SYSTEM_INFO));
   GetSystemInfo(&sysinfo);

   const uint64_t cpuThreadCount = sysinfo.dwNumberOfProcessors;
   std::string cpuInfo = cpuName + " (" + std::to_string(cpuThreadCount);

   if (cpuThreadCount == 1)
      cpuInfo.append(" thread)");
   else
      cpuInfo.append(" threads)");

   return cpuInfo;
}

std::string get_os_version()
{
   OSVERSIONINFOEX osvi;

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   bool os_version_info_ex = GetVersionEx((OSVERSIONINFO*) &osvi);

   if (not os_version_info_ex) 
   {
      osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      if (not GetVersionEx((OSVERSIONINFO*) &osvi))
         return "";
   }

   if (osvi.dwMajorVersion    == 5 and 
       osvi.dwMinorVersion    == 1 and
       osvi.wServicePackMajor == 2 and
       osvi.wServicePackMinor == 0)
      return "Microsoft Windows XP, Service Pack 2";

   if (osvi.dwMajorVersion    == 5 and 
       osvi.dwMinorVersion    == 1 and
       osvi.wServicePackMajor == 3 and
       osvi.wServicePackMinor == 0)
      return "Microsoft Windows XP, Service Pack 3";

   if (osvi.dwMajorVersion    == 6 and 
       osvi.dwMinorVersion    == 0 and
       osvi.wServicePackMajor == 1 and
       osvi.wServicePackMinor == 0)
      return "Microsoft Windows Vista, Service Pack 1";

   if (osvi.dwMajorVersion    == 6 and 
       osvi.dwMinorVersion    == 0 and
       osvi.wServicePackMajor == 2 and
       osvi.wServicePackMinor == 0)
      return "Microsoft Windows Vista, Service Pack 2";

   if (osvi.dwMajorVersion    == 6 and 
       osvi.dwMinorVersion    == 0)
      return "Microsoft Windows Vista";

   if (osvi.dwMajorVersion    == 6 and 
       osvi.dwMinorVersion    == 1 and
       osvi.wServicePackMajor == 1 and
       osvi.wServicePackMinor == 0)
      return "Microsoft Windows 7, Service Pack 1";

   if (osvi.dwMajorVersion    == 6 and 
       osvi.dwMinorVersion    == 1)
      return "Microsoft Windows 7";

   if (osvi.dwMajorVersion    == 6 and 
       osvi.dwMinorVersion    == 2)
      return "Microsoft Windows 8";

   if (osvi.dwMajorVersion    == 6 and 
       osvi.dwMinorVersion    == 3)
      return "Microsoft Windows 8.1";

   return "Microsoft Windows";
}

std::string get_host_name()
{
   char hostname[100];
   DWORD size = sizeof (hostname);
   bool hostname_fail = not GetComputerName(hostname, &size);

   return hostname_fail ? "localhost" : hostname;
}

std::string get_user_name()
{
/*
   uint32_t len = UNLEN + 1;
   wchar_t  buffer[UNLEN + 1];
    
   if (GetUserNameW(buffer, (LPDWORD) &len))
   {
      // wide to UTF-8
      std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
      return conv.to_bytes(buffer);
   }
*/   
   uint32_t len = UNLEN + 1;
   char buffer[UNLEN + 1];
    
   if (GetUserName(buffer, (LPDWORD) &len))
   {
      return buffer;
   }
   return "";
}

int get_process_id()
{
   return getpid();
}

std::string get_program_name()
{
   HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION |
                                       PROCESS_VM_READ,
                                       FALSE, getpid());
   if (NULL == process_handle)
       return "";

   TCHAR module_name[MAX_PATH];

   GetModuleFileNameEx(process_handle,
                           NULL,
                           module_name, sizeof(module_name) / sizeof(TCHAR)); 

   CloseHandle(process_handle);

   return module_name;
}

} // namespace systeminfo
} // namespace orion
