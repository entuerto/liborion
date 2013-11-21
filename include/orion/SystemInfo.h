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

#include <string>
#include <vector>


namespace orion
{
namespace systeminfo
{

typedef std::vector<std::string> ModuleList;

std::string get_cpu_info();

std::string get_os_version();

std::string get_host_name();

std::string get_user_name();

int get_process_id();

std::string get_program_name();

void get_loaded_modules(unsigned long process_id, ModuleList& modules);

}
} // namespace orion
#endif // ORION_SYSTEMINFO_H
