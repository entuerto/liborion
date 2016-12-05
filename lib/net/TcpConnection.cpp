//
// TcpConnection.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <orion/net/Connection.h>

namespace orion
{
namespace net
{
//---------------------------------------------------------------------------------------
// TcpConnection

TcpConnection::TcpConnection():
   _local_addr(),
   _remote_addr(),
   _read_deadline(),
   _write_deadline(),
   _keep_alive(true),
   _no_delay(true)
{
}

TcpConnection::~TcpConnection()
{
}

IPAddress* TcpConnection::local_addr() const
{
   return _local_addr.get();
}

IPAddress* TcpConnection::remote_addr() const
{
   return _remote_addr.get();
}

std::error_code TcpConnection::deadline(const std::chrono::seconds& sec)
{
   _read_deadline  = sec;
   _write_deadline = sec;
   return std::error_code();
}

std::chrono::seconds TcpConnection::deadline() const
{
   return _read_deadline;
}

std::error_code TcpConnection::read_deadline(const std::chrono::seconds& sec)
{
   _read_deadline  = sec;
   return std::error_code();
}

std::chrono::seconds TcpConnection::read_deadline() const
{
   return _read_deadline;
}

std::error_code TcpConnection::write_deadline(const std::chrono::seconds& sec)
{
   _write_deadline = sec;
   return std::error_code();
}

std::chrono::seconds TcpConnection::write_deadline() const
{
   return _write_deadline;
}

std::error_code TcpConnection::keep_alive(bool value)
{
   _keep_alive = value;
   return std::error_code();
}

bool TcpConnection::keep_alive() const
{
   return _keep_alive;
}

std::error_code TcpConnection::no_delay(bool value)
{
   _no_delay = value;
   return std::error_code();
}

bool TcpConnection::no_delay() const
{
   return _no_delay;
}

} // net
} // orion
