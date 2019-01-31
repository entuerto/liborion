//
// AsyncService.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_ASYNCSERVICE_H
#define ORION_ASYNCSERVICE_H

#include <orion/Orion-Stddefs.h>

#include <asio.hpp>

#include <list>
#include <vector>

namespace orion
{
class API_EXPORT AsyncService
{
public:
   NO_COPY(AsyncService) 

   explicit AsyncService(std::size_t pool_size = 1);
   ~AsyncService();

   /// Run all IOService objects.
   void run();

   /// Stop all IOService objects.
   void stop() noexcept;

   /// Get an IOService to use.
   asio::io_context& io_context();

private:
   using io_context_work = asio::executor_work_guard<asio::io_context::executor_type>;
   
   /// The pool of io_contexts.
   std::vector<std::shared_ptr<asio::io_context>> _io_contexts;

   /// The work that keeps the io_contexts running.
   std::list<io_context_work> _work;

   /// The next io_context to use for a connection.
   std::size_t _next_io_context{0u};
};

} // orion
#endif // ORION_ASYNCSERVICE_H
