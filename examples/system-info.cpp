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

#include <orion/SystemInfo.h>

#include <cstdio>
#include <iostream>
#include <sstream>

using namespace orion;
using namespace orion::systeminfo;

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

   for (auto it = cpu_infos.begin(); it != cpu_infos.end(); it++)
   {
      std::cout << "  Model:   " << it->model() << "\n"
                << "  Speed:   " << it->speed() << "\n"
                << "  Times"
                << "\n"
                << "     user:  " << it->times().user << "\n"
                << "     nice:  " << it->times().nice << "\n"
                << "     sys:   " << it->times().sys << "\n"
                << "     idle:  " << it->times().idle << "\n"
                << "     irq:   " << it->times().irq << "\n"
                << "\n";
   }

   std::cout << "\n"
             << "Program Information\n"
             << "  " << get_program_name() << "\n";

   return EXIT_SUCCESS;
}
