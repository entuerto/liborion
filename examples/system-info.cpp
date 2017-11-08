//
// system-info.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/System.h>

#include <cstdio>
#include <iostream>
#include <sstream>

using namespace orion;
using namespace orion::sys;

void function_a()
{
   write_stack_trace(std::cout);
}

int main()
{
   std::cout << "System Information\n"
             << "  Name:       " << get_user_name() << "\n"
             << "  Host name:  " << get_host_name() << "\n"
             << "  Process ID: " << get_process_id() << "\n"
             << "  Sytem:      " << get_os_version() << "\n"
             << "  CPU:        " << get_cpu_model() << "\n"
             << "\n"
             << "Memory\n"
             << "  Free:       " << human_readable(get_free_memory()) << "\n"
             << "  Total:      " << human_readable(get_total_memory()) << "\n"
             << "\n"
             << "Processor\n";

   std::vector<CpuInfo> cpu_infos = get_cpu_info();

   for (const auto& cpu : cpu_infos)
   {
      std::cout << "  Model:   " << cpu.model << "\n"
                << "  Speed:   " << cpu.speed << "\n"
                << "  Times"
                << "\n"
                << "     user:  " << cpu.times.user << "\n"
                << "     nice:  " << cpu.times.nice << "\n"
                << "     sys:   " << cpu.times.sys << "\n"
                << "     idle:  " << cpu.times.idle << "\n"
                << "     irq:   " << cpu.times.irq << "\n"
                << "\n";
   }

   std::cout << "\n"
             << "Program Information\n"
             << "  " << get_program_name() << "\n";

   function_a();

   return EXIT_SUCCESS;
}
