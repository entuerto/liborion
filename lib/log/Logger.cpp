//
// Logger.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/log/Logger.h>

#include <orion/Log.h>
#include <orion/Utils.h>

#include <orion/System.h>
#include <sstream>

namespace orion
{
namespace log
{
//-------------------------------------------------------------------------------------------------

Logger& default_logger()
{
   try
   {
      static asio::io_context private_service;
      static Logger logger(private_service);

      return logger;
   }
   catch (...)
   {
      log::error(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", _src_loc);
      std::terminate();
   }
}

void start(SystemInfoFunc system_info /* = DefaultInfo */)
{
   default_logger().start(std::move(system_info));
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
