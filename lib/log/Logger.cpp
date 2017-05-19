//
// Logger.cpp
//
// Created by Tomas Palazuelos on 2017-03-17.
// Copyright © 2017 Tomas Palazuelos. All rights reserved.
//

#include <orion/log/Logger.h>

#include <orion/SystemInfo.h>
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
               << "  Name:       " << systeminfo::get_user_name() << "\n"
               << "  Host name:  " << systeminfo::get_host_name() << "\n"
               << "  Process ID: " << systeminfo::get_process_id() << "\n"
               << "  Sytem:      " << systeminfo::get_os_version() << "\n"
               << "  CPU:        " << systeminfo::get_cpu_model() << "\n"
               << "\n"
               << "Program Information\n"
               << "  " << systeminfo::get_program_name() << "\n";

   return stream_info.str();
}

} // namespace log
} // namespace orion
