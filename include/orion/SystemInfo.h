// SystemInfo.h
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

#ifndef ORION_SYSTEMINFO_H
#define ORION_SYSTEMINFO_H

#include <cstdint>
#include <string>
#include <vector>
#include <orion/Orion-Stddefs.h>

namespace orion
{
namespace systeminfo
{
struct CpuTimes
{
   uint32_t user;
   uint32_t nice;
   uint32_t sys;
   uint32_t idle;
   uint32_t irq;
};

class API_EXPORT CpuInfo
{
public:
   CpuInfo(const std::string& model, uint32_t speed, CpuTimes& times);
   ~CpuInfo();

   std::string model() const;
   uint32_t    speed() const;
   CpuTimes    times() const;

   std::string to_string() const;

private:
   std::string _model;
   uint32_t _speed;
   CpuTimes _times; 
};

typedef std::vector<std::string> ModuleList;

API_EXPORT std::string human_readable(uint64_t value, uint64_t base = 1024);

API_EXPORT std::string get_cpu_model();

API_EXPORT std::vector<CpuInfo> get_cpu_info();

API_EXPORT std::string get_os_version();

API_EXPORT std::string get_host_name();

API_EXPORT std::string get_user_name();

API_EXPORT int get_process_id();

API_EXPORT std::string get_program_name();

API_EXPORT void get_loaded_modules(unsigned long process_id, ModuleList& modules);

API_EXPORT void get_loadavg(double avg[3]);

API_EXPORT uint64_t get_free_memory();

API_EXPORT uint64_t get_total_memory();

}
} // namespace orion
#endif // ORION_SYSTEMINFO_H
