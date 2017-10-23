// AsyncService.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#ifndef ORION_ASYNCSERVICE_H
#define ORION_ASYNCSERVICE_H

#include <orion/Orion-Stddefs.h>

namespace orion 
{
///
/// A pool of io_service objects.
/// 
class AsyncService
{
public:
   static void run();
};

} // namespace orion

#endif // ORION_ASYNCSERVICE_H