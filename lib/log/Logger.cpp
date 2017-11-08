//
// Logger.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/log/Logger.h>

#include <orion/System.h>
#include <sstream>

namespace orion
{
namespace log
{
static asio::io_service private_service;

//-------------------------------------------------------------------------------------------------

Logger& default_logger()
{
   static Logger logger(private_service);

   return logger;
}

void start(SystemInfoFunc system_info /* = DefaultInfo */)
{
   default_logger().start(system_info);
}

void shutdown()
{
   default_logger().shutdown();
}

std::string DefaultInfo()
{
   std::ostringstream stream_info;

   stream_info << "System Information\n"
               << "  Name:       " << sys::get_user_name() << "\n"
               << "  Host name:  " << sys::get_host_name() << "\n"
               << "  Process ID: " << sys::get_process_id() << "\n"
               << "  Sytem:      " << sys::get_os_version() << "\n"
               << "  CPU:        " << sys::get_cpu_model() << "\n"
               << "\n"
               << "Program Information\n"
               << "  " << sys::get_program_name() << "\n";

   return stream_info.str();
}

} // namespace log
} // namespace orion
