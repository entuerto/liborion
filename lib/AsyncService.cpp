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

#include <asio.hpp>

namespace orion 
{

IOService AsyncService::io_service;

void AsyncService::run()
{
   std::error_code ec;;
   
   AsyncService::io_service.run(ec);

   if (ec)
      log::error("AsyncService::run() ", ec);
}

} // namespace orion
