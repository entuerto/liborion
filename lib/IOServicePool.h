// IOServicePool.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// ------------------------------------------------------------------------------------------------
// I wrote this code based on the original code which has the
// following license:
//
// io_service_pool.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef ORION_IOSERVICEPOOL_H
#define ORION_IOSERVICEPOOL_H

#include <orion/Orion-Stddefs.h>

#include <asio.hpp>

#include <memory>
#include <vector>


namespace orion 
{
///
/// A pool of io_service objects.
/// 
class IOServicePool
{
public:
   NO_COPY(IOServicePool)

   /// Construct the io_service pool.
   explicit IOServicePool(std::size_t pool_size);

   /// Run all io_service objects in the pool.
   void run();

   /// Stop all io_service objects in the pool.
   void stop();

   /// Get an io_service to use.
   asio::io_service& get_io_service();

private:
   using io_service_ptr = std::shared_ptr<asio::io_service>;
   using work_ptr = std::shared_ptr<asio::io_service::work>;

   /// The pool of io_services.
   std::vector<io_service_ptr> _io_services;

   /// The work that keeps the io_services running.
   std::vector<work_ptr> _work;

   /// The next io_service to use for a connection.
   std::size_t _next_io_service;
};

} // namespace orion

#endif // ORION_IOSERVICEPOOL_H