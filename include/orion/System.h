//
// System.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_SYSTEM_H
#define ORION_SYSTEM_H

#include <orion/Orion-Stddefs.h>

#include <orion/Units.h>

#include <cstdint>
#include <string>
#include <vector>

namespace orion
{
namespace sys
{
   
struct CpuTimes
{
   uint32_t user;
   uint32_t nice;
   uint32_t sys;
   uint32_t idle;
   uint32_t irq;
};

struct CpuInfo
{
   std::string id;
   std::string name;
   std::string vendor;
   uint32_t    speed;
   uint32_t    cpu_count;
   uint32_t    cpu_count_phys;
};

using ModuleList = std::vector<std::string>;

API_EXPORT CpuInfo get_cpu_info();

API_EXPORT CpuTimes get_cpu_times();

API_EXPORT std::string get_os_version();

API_EXPORT std::string get_host_name();

API_EXPORT std::string get_user_name();

API_EXPORT int get_process_id();

API_EXPORT std::string get_program_name();

API_EXPORT void get_loaded_modules(unsigned long process_id, ModuleList& modules);

API_EXPORT void get_loadavg(double avg[3]);

API_EXPORT Size<si::Byte> get_free_memory();

API_EXPORT Size<si::Byte> get_total_memory();

} // namespace sys
} // namespace orion
#endif // ORION_SYSTEM_H
