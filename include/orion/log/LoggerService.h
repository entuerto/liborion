//
// LoggerService.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_LOGGERSERVICE_H
#define ORION_LOG_LOGGERSERVICE_H

#include <orion/Orion-Stddefs.h>

#include <asio.hpp>

#include <string>

namespace orion
{
namespace log
{
class OutputHandler;
class Record;

/// Service implementation for the logger.
template<typename LoggerImpl>
class LoggerService : public asio::io_context::service
{
public:
   /// The unique service identifier.
   static asio::io_context::id id;

   /// The type for an implementation of the logger.
   using ImplType = std::shared_ptr<LoggerImpl>;

   explicit LoggerService(asio::io_context& io_context)
      : asio::io_context::service(io_context)
      , _work_io_context()
      , _work(asio::make_work_guard(_work_io_context))
      , _work_thread([&]() { _work_io_context.run(); })
   {
   }

   LoggerService(const LoggerService&) = default;
   LoggerService(LoggerService&&)      = default;

   ~LoggerService() override
   {
      /// Indicate that we have finished with the private io_context. Its
      /// io_context::run() function will exit once all other work has completed.
      _work.reset();
      _work_thread.join();
   }

   LoggerService& operator=(const LoggerService&) = default;
   LoggerService& operator=(LoggerService&&) = default;

   /// Create a new logger implementation.
   void create(ImplType& impl) { impl.reset(new LoggerImpl(Level::Warning)); }

   void destroy(ImplType& impl) { impl.reset(); }

   /// Get the logging level
   Level level(const ImplType& impl) const { return impl->level(); }

   /// Set the logging level
   void level(ImplType& impl, Level value) { impl->level(value); }

   bool is_enabled(const ImplType& impl, Level level) const { return impl->is_enabled(level); }

   /// Indicates if the service is suspended
   bool is_running(const ImplType& impl) const { return impl->is_running(); }

   void is_running(ImplType& impl, bool value) { impl->is_running(value); }

   void add_output_handler(ImplType& impl, std::unique_ptr<OutputHandler>&& hdl)
   {
      impl->add_output_handler(std::move(hdl));
   }

   /// Write a record to the output handlers
   void write(ImplType& impl, const Record& record) { impl->write(record); }

   /// Starts the logging
   void start(ImplType& impl, SystemInfoFunc system_info) { impl->start(system_info); }

   /// Shuts down the logger
   void shutdown_logger(ImplType& impl) { impl->shutdown(); }

   /// Suspend logging
   void suspend(ImplType& impl) { impl->suspend(); }

   /// Resume logging
   void resume(ImplType& impl) { impl->resume(); }

   void push_scope(ImplType& impl) { impl->push_scope(); }

   void pop_scope(ImplType& impl) { impl->pop_scope(); }

   uint32_t scope_depth(const ImplType& impl) const { return impl->scope_depth(); }

private:
   /// Destroy all user-defined handler objects owned by the service.
   //void shutdown_service() override {}

private:
   /// Private io_context used for performing logging operations.
   asio::io_context _work_io_context;

   /// Work for the private io_context to perform. If we do not give the
   /// io_context some work to do then the io_context::run() function will exit
   /// immediately.
   asio::executor_work_guard<asio::io_context::executor_type> _work;

   /// Thread used for running the work io_context's run loop.
   asio::thread _work_thread;
};

template<typename LoggerImpl>
asio::io_context::id LoggerService<LoggerImpl>::id;

} // namespace log
} // namespace orion

#endif /* ORION_LOG_LOGGERSERVICE_H */
