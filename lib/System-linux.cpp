//
// System-linux.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/System.h>

#include <orion/StringUtils.h>

#include <sys/utsname.h>
#include <unistd.h>

#include <fstream>
#include <sstream>

namespace orion
{
namespace sys
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
         cpus.push_back(CpuInfo{trim(tokens.back()), 1000, CpuTimes & times});
   }

   return cpus;
}

std::string get_os_version()
{
   struct utsname name;

   if (uname(&name) == -1)
      return "";

   std::ostringstream os_version;

   os_version << name.sysname << " " << name.release << " " << name.version;
   return os_version.str();
}

std::string get_host_name()
{
   char hostname[100];
   bool hostname_fail = gethostname(hostname, sizeof(hostname)) == -1;

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

   avg[0] = (double)info.loads[0] / 65536.0;
   avg[1] = (double)info.loads[1] / 65536.0;
   avg[2] = (double)info.loads[2] / 65536.0;
}

uint64_t get_free_memory()
{
   return static_cast<uint64_t>(sysconf(_SC_PAGESIZE) * sysconf(_SC_AVPHYS_PAGES));
}

uint64_t get_total_memory()
{
   return static_cast<uint64_t>(sysconf(_SC_PAGESIZE) * sysconf(_SC_PHYS_PAGES));
}

} // namespace sys
} // namespace orion
