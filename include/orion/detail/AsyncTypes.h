//
// AsyncTypes.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_ASYNCTYPES_H
#define ORION_ASYNCTYPES_H

#include <orion/Config.h>

#include <asio.hpp>

namespace orion
{

using IOService = asio::io_service;
using SignalSet = asio::signal_set;

using SteadyTimer = asio::steady_timer;

using StreamBuffer = asio::streambuf;

using TcpSocket = asio::ip::tcp::socket;
} // orion
#endif // ORION_ASYNCTYPES_H
