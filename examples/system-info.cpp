//
// system-info.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/System.h>
#include <orion/debug/Stacktrace.h>

#include <fmt/format.h>

#include <cstdio>
#include <iostream>
#include <sstream>

using namespace orion;
using namespace orion::debug;
using namespace orion::log;
using namespace orion::sys;

void function_a()
{
   Stacktrace st = make_stacktrace();

   std::cout << to_string(st);
}

void setup_logger()
{
   auto cout_handler = make_stream_output_handler(std::cout);

   Logger& logger = default_logger();

   logger.level(Level::Debug);
   logger.add_output_handler(std::move(cout_handler));
}

std::string get_model(const CpuInfo& cpu)
{
   return (cpu.cpu_count == 1) 
             ? fmt::format("{0} ({1} thread)", cpu.name, cpu.cpu_count)
             : fmt::format("{0} ({1} threads)", cpu.name, cpu.cpu_count);
}

int main()
{
   setup_logger();

   log::start([]() { return std::string{}; });

   CpuInfo cpu = get_cpu_info();

   std::cout << "System Information\n"
             << "  Name:       " << get_user_name() << "\n"
             << "  Host name:  " << get_host_name() << "\n"
             << "  Process ID: " << get_process_id() << "\n"
             << "  Sytem:      " << get_os_version() << "\n"
             << "  CPU:        " << get_model(cpu) << "\n"
             << "\n"
             << "Memory\n"
             << "  Free:       " << static_cast<Value<iec::Gibibyte>>(get_free_memory()) << "\n"
             << "  Total:      " << static_cast<Value<iec::Gibibyte>>(get_total_memory()) << "\n"
             << "\n";
             
   std::cout << "Processor\n"
             << "  Id:           " << cpu.id << "\n"
             << "  Vendor:       " << cpu.vendor << "\n"
             << "  Model:        " << cpu.name << "\n"
             << "  Speed:        " << cpu.speed << "\n"
             << "  Count:        " << cpu.cpu_count << "\n"
             << "  Count (phys): " << cpu.cpu_count_phys << "\n";

   CpuTimes cpu_times = get_cpu_times();

   std::cout << "  Times\n"
             << "     user:  " << cpu_times.user << "\n"
             << "     nice:  " << cpu_times.nice << "\n"
             << "     sys:   " << cpu_times.sys << "\n"
             << "     idle:  " << cpu_times.idle << "\n"
             << "     irq:   " << cpu_times.irq << "\n"
             << "\n";

   std::cout << "Program Information\n"
             << "  " << get_program_name() << "\n\n";

   function_a();

   log::shutdown();
   return EXIT_SUCCESS;
}
