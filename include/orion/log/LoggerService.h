// LoggerService.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef ORION_LOG_LOGGERSERVICE_H
#define ORION_LOG_LOGGERSERVICE_H

#include <string>
#include <thread>

#include <orion/Orion-Stddefs.h>

#include <asio.hpp>

namespace orion
{
namespace log
{
class OutputHandler;
class Record;

/// Service implementation for the logger.
template <typename LoggerImplementation>
class LoggerService : public asio::io_service::service
{
public:
   /// The unique service identifier.
   static asio::io_service::id id;

   /// The type for an implementation of the logger.
   typedef std::shared_ptr<LoggerImplementation> implementation_type;

   explicit LoggerService(asio::io_service& io_service) : 
      asio::io_service::service(io_service),
      _work_io_service(),
      _work(std::make_unique<asio::io_service::work>(_work_io_service)),
      _work_thread([&](){ _work_io_service.run(); })
   {
   }
   
   ~LoggerService()
   {
      // The work thread will finish when _work is reset as all asynchronous 
      // operations have been aborted and were discarded before (in destroy). 
      _work.reset(nullptr);

      // Event processing is stopped to discard queued operations. 
      _work_io_service.stop();

      /// Indicate that we have finished with the private io_service. Its
      /// io_service::run() function will exit once all other work has completed.
      _work_thread.join();
   }

   void construct(implementation_type& impl)
   {
      impl.reset(new LoggerImplementation(Level::Warning)); 
   }

   void destroy(implementation_type& impl)
   {
      impl.reset();
   }

   /// Get the logging level
   Level level(const implementation_type& impl) const
   {
      return impl->level();
   }

   /// Set the logging level
   void level(implementation_type& impl, Level value)
   {
      impl->level(value);
   }

   bool is_enabled(const implementation_type& impl, Level level) const
   {
      return impl->is_enabled(level);
   }

   /// Indicates if the service is suspended
   bool is_running(const implementation_type& impl) const
   {
      return impl->is_running();
   }

   void is_running(implementation_type& impl, bool value)
   {
      impl->is_running(value);
   }

   void add_output_handler(implementation_type& impl, std::unique_ptr<OutputHandler>&& hdl)
   {
      impl->add_output_handler(std::move(hdl));
   }

   /// Write a record to the output handlers
   void write(implementation_type& impl, const Record& record)
   {
      impl->write(record);
   }

   /// Starts the logging
   void start(implementation_type& impl, SystemInfoFunc system_info)
   {
      impl->start(system_info);
   }

   /// Shuts down the logger
   void shutdown(implementation_type& impl)
   {
      impl->shutdown();
   }

   /// Suspend logging
   void suspend(implementation_type& impl)
   {
      impl->suspend();
   }

   /// Resume logging
   void resume(implementation_type& impl)
   {
      impl->resume();
   }

   void push_scope(implementation_type& impl)
   {
      impl->push_scope();
   }

   void pop_scope(implementation_type& impl)
   {
      impl->pop_scope();
   }

   uint32_t scope_depth(const implementation_type& impl) const
   {
      return impl->scope_depth();
   }

private:
   /// Destroy all user-defined handler objects owned by the service.
   void shutdown_service() {}

private:
   /// Private io_service used for performing logging operations.
   asio::io_service _work_io_service;

   /// Work for the private io_service to perform. If we do not give the
   /// io_service some work to do then the io_service::run() function will exit
   /// immediately.
   std::unique_ptr<asio::io_service::work> _work;

   /// Thread used for running the work io_service's run loop.
   std::thread _work_thread;
};

template <typename LoggerImplementation>
asio::io_service::id LoggerService<LoggerImplementation>::id;

} // namespace log
} // namespace orion

#endif /* ORION_LOG_LOGGERSERVICE_H */