// AsyncService.cpp
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// ------------------------------------------------------------------------------------------------
// I wrote this code based on the original code which has the
// following license:
//
#include <orion/AsyncService.h>

#include <orion/Log.h>

#include <AsyncServicePriv.h>

#include <asio.hpp>

namespace orion 
{

namespace detail
{
static asio::io_service io_service;
}

asio::io_service& get_main_io_service()
{
   return detail::io_service;
}

void AsyncService::run()
{
   std::error_code ec;;
   
   detail::io_service.run(ec);

   if (ec)
      log::error("AsyncService::run() ", ec);
}

} // namespace orion
