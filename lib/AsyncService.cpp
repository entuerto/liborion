//
// AsyncService.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
// ------------------------------------------------------------------------------------------------
// I wrote this code based on the original code which has the
// following license:
//
#include <orion/AsyncService.h>

#include <orion/Log.h>

#include <asio.hpp>

#include <stdexcept>
#include <thread>

namespace orion
{

AsyncService::AsyncService(std::size_t pool_size /* = 1 */)
   : _next_io_context(0)
{
   if (pool_size == 0)
   {
      throw std::runtime_error("AsyncService pool size is 0");
   }

   // Give all the io_contexts work to do so that their run() functions will not
   // exit until they are explicitly stopped.
   for (std::size_t i = 0; i < pool_size; ++i)
   {
      auto io_context = std::make_shared<asio::io_context>();
      auto work       = std::make_shared<asio::io_context::work>(*io_context);

      _io_contexts.push_back(io_context);
      _work.push_back(work);
   }
}

AsyncService::~AsyncService()
{
   stop();
}

void AsyncService::run()
{
   log::debug2("AsyncService::run()");

   // Create a pool of threads to run all of the io_contexts.
   std::vector<std::shared_ptr<std::thread>> threads;

   for (auto& service : _io_contexts)
   {
      auto thread = std::make_shared<std::thread>([service]() {
         std::error_code ec;
         service->run(ec);
         if (ec)
         {
            log::error(ec);
         }
      });
      threads.push_back(thread);
   }

   // Wait for all threads in the pool to exit.
   for (auto& t : threads)
   {
      t->join();
   }
}

void AsyncService::stop()
{
   // Destroy all work objects to signals end of work
   _work.clear();
}

asio::io_context& AsyncService::io_context()
{
   // Use a round-robin scheme to choose the next io_context to use.
   asio::io_context& io_context = *_io_contexts[_next_io_context];

   ++_next_io_context;

   if (_next_io_context == _io_contexts.size())
   {
      _next_io_context = 0;
   }

   return io_context;
}

} // namespace orion
