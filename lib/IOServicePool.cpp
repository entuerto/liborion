// IOServicePool.cpp
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// ------------------------------------------------------------------------------------------------
// I wrote this code based on the original code which has the
// following license:
//
// IOServicePool.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <IOServicePool.h>

#include <stdexcept>
#include <thread>

namespace orion 
{

IOServicePool::IOServicePool(std::size_t pool_size)
   : _next_io_service(0)
{
   if (pool_size == 0)
      throw std::runtime_error("IOServicePool size is 0");

   // Give all the io_services work to do so that their run() functions will not
   // exit until they are explicitly stopped.
   for (std::size_t i = 0; i < pool_size; ++i)
   {
      io_service_ptr io_service = std::make_shared<asio::io_service>();
      _io_services.push_back(io_service);
      
      work_ptr work = std::make_shared<asio::io_service::work>(*io_service);
      _work.push_back(work);
   }
}

void IOServicePool::run()
{
   // Create a pool of threads to run all of the io_services.
   std::vector<std::shared_ptr<std::thread>> threads;

   for (std::size_t i = 0; i < _io_services.size(); ++i)
   {
      std::shared_ptr<std::thread> thread = std::make_shared<std::thread>([&]() { _io_services[i]->run(); });
      threads.push_back(thread);
   }

   // Wait for all threads in the pool to exit.
   for (auto& t : threads)
      t->join();
}

void IOServicePool::stop()
{
   // Explicitly stop all io_services.
   for (auto& ios : _io_services)
      ios->stop();
}

asio::io_service& IOServicePool::get_io_service()
{
   // Use a round-robin scheme to choose the next io_service to use.
   asio::io_service& io_service = *_io_services[_next_io_service];

   ++_next_io_service;

   if (_next_io_service == _io_services.size())
      _next_io_service = 0;

   return io_service;
}

} // namespace orion
