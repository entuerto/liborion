// AsyncServicePriv.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#ifndef ORION_ASYNCSERVICEPRIV_H
#define ORION_ASYNCSERVICEPRIV_H

#include <orion/Orion-Stddefs.h>

#include <asio.hpp>

namespace orion 
{
///
/// Default io_service object.
/// 
asio::io_service& get_main_io_service();

} // namespace orion

#endif // ORION_ASYNCSERVICE_H