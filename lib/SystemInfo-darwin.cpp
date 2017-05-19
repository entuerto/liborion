// SystemInfo.cpp
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

#include <orion/StringUtils.h>

#include <cstdlib>
#include <sys/sysctl.h>
#include <sys/utsname.h>
#include <unistd.h>

#include <mach-o/dyld.h>
#include <mach/mach.h>

#include <fstream>
#include <sstream>

namespace orion
{
namespace systeminfo
{

void get_loaded_modules(unsigned long /* process_id */, ModuleList& /* modules */)
{
}

std::string get_cpu_model()
{
   char buffer[100];
   size_t len = 100;

   sysctlbyname("machdep.cpu.brand_string", &buffer, &len, nullptr, 0);

   return buffer;
}

std::vector<CpuInfo> get_cpu_info()
{
    return std::vector<CpuInfo>();
}

std::string get_os_version()
{
   struct utsname name;

   if (uname(&name) == -1)
      return "";

   return name.version;
}

std::string get_host_name()
{
   char hostname[100];
   bool hostname_fail = gethostname(hostname, sizeof(hostname)) == -1;

   return hostname_fail ? "localhost" : hostname;
}

std::string get_user_name()
{
   return getenv("USER");
}

int get_process_id()
{
   return getpid();
}

std::string get_program_name()
{
   char buffer[1024];
   uint32_t len = 1024;

   _NSGetExecutablePath(buffer, &len);
   return buffer;
}

void get_loadavg(double avg[3])
{
   struct loadavg info;
   size_t size = sizeof(info);
   int which[] = {CTL_VM, VM_LOADAVG};

   if (sysctl(which, 2, &info, &size, NULL, 0) < 0)
      return;

   avg[0] = static_cast<double>(info.ldavg[0] / info.fscale);
   avg[1] = static_cast<double>(info.ldavg[1] / info.fscale);
   avg[2] = static_cast<double>(info.ldavg[2] / info.fscale);
}

uint64_t get_free_memory()
{
   vm_statistics_data_t info;
   mach_msg_type_number_t count = sizeof(info) / sizeof(integer_t);

   if (host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&info, &count) != KERN_SUCCESS)
   {
      return -EINVAL; /* FIXME: Translate error. */
   }

   return (uint64_t)info.free_count * sysconf(_SC_PAGESIZE);
}

uint64_t get_total_memory()
{
   uint64_t info;
   int which[] = {CTL_HW, HW_MEMSIZE};
   size_t size = sizeof(info);

   if (sysctl(which, 2, &info, &size, NULL, 0))
      return -errno;

   return (uint64_t)info;
}

} // namespace systeminfo
} // namespace orion
