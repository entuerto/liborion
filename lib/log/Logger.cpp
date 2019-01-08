//
// Logger.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/log/Logger.h>

#include <orion/Log.h>
#include <orion/System.h>
#include <orion/Utils.h>

#include <fmt/format.h>

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

void setup_logger(Level level /* = Level::Info */)
{
   auto cout_handler = make_stream_output_handler(std::cout);

   Logger& logger = default_logger();

   logger.level(level);
   logger.add_output_handler(std::move(cout_handler));
}

void setup_logger(Level level, std::ostream& stream)
{
   auto stream_handler = make_stream_output_handler(stream);

   Logger& logger = default_logger();

   logger.level(level);
   logger.add_output_handler(std::move(stream_handler));
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

   sys::CpuInfo cpu = sys::get_cpu_info();

   auto cpu_name = (cpu.cpu_count == 1) 
                      ? fmt::format("{0} ({1} thread)", cpu.name, cpu.cpu_count)
                      : fmt::format("{0} ({1} threads)", cpu.name, cpu.cpu_count); 

   stream_info << "System Information\n"
               << "  Name:       " << sys::get_user_name() << "\n"
               << "  Host name:  " << sys::get_host_name() << "\n"
               << "  Process ID: " << sys::get_process_id() << "\n"
               << "  Sytem:      " << sys::get_os_version() << "\n"
               << "  CPU:        " << cpu_name << "\n"
               << "\n"
               << "Program Information\n"
               << "  " << sys::get_program_name() << "\n";

   return stream_info.str();
}

} // namespace log
} // namespace orion
