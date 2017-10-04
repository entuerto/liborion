// main.cpp
//
// Copyright 2009 tomas <tomasp@videotron.ca>
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
