// SystemInfo-linux.cpp
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

#include <unistd.h>
#include <sys/utsname.h>

#include <fstream>
#include <sstream>

#include <orion/StringUtils.h>

namespace orion
{
namespace systeminfo
{

void get_loaded_modules(unsigned long /* process_id */, ModuleList& /* modules */)
{

}

std::string get_cpu_model()
{
   return "";
}

std::vector<CpuInfo> get_cpu_info()
{
   std::ifstream f("/proc/cpuinfo");

   std::string line;
   std::vector<std::string> tokens;
   std::vector<CpuInfo> cpus;

   while (not f.eof() and not f.fail())
   {
      std::getline(f, line);

      tokens = split(line, ':');

      if (tokens.size() == 2 and trim(tokens.front()) == "model name")
         cpus.push_back(CpuInfo(trim(tokens.back()), 1000, CpuTimes& times));
   }

   return cpus;
}

std::string get_os_version()
{
   struct utsname name;

   if (uname (&name) == -1)
      return "";

   std::ostringstream os_version;

   os_version << name.sysname
              << " "
              << name.release
              << " "
              << name.version;
   return os_version.str();
}

std::string get_host_name()
{
   char hostname[100];
   bool hostname_fail = gethostname(hostname, sizeof (hostname)) == -1;

   return hostname_fail ? "localhost" : hostname;
}

std::string get_user_name()
{
   return "";
}

int get_process_id()
{
   return getpid();
}

std::string get_program_name() 
{
   return "";
}

void get_loadavg(double avg[3]) 
{
   struct sysinfo info;

   if (sysinfo(&info) < 0) 
      return;

   avg[0] = (double) info.loads[0] / 65536.0;
   avg[1] = (double) info.loads[1] / 65536.0;
   avg[2] = (double) info.loads[2] / 65536.0;
}

uint64_t get_free_memory() 
{
   return static_cast<uint64_t>(sysconf(_SC_PAGESIZE) * sysconf(_SC_AVPHYS_PAGES));
}

uint64_t get_total_memory() 
{
   return static_cast<uint64_t>(sysconf(_SC_PAGESIZE) * sysconf(_SC_PHYS_PAGES));
}


} // namespace systeminfo
} // namespace orion
