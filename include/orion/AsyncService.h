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
   void stop();

   /// Get an IOService to use.
   asio::io_service& io_service();

private:
   /// The pool of io_services.
   std::vector<std::shared_ptr<asio::io_service>> _io_services;

   /// The work that keeps the io_services running.
   std::vector<std::shared_ptr<asio::io_service::work>> _work;

   /// The next io_service to use for a connection.
   std::size_t _next_io_service;
};

} // orion
#endif // ORION_ASYNCSERVICE_H
