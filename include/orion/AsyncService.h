// AsyncService.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#ifndef ORION_ASYNCSERVICE_H
#define ORION_ASYNCSERVICE_H

#include <orion/Orion-Stddefs.h>

#include <orion/detail/AsyncTypes.h>

namespace orion 
{
///
/// A pool of io_service objects.
/// 
struct API_EXPORT AsyncService
{
   static IOService io_service;

   static void run();
};

} // namespace orion

#endif // ORION_ASYNCSERVICE_H