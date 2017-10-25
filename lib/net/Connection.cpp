//
// Connection.cpp
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
// Connection

Connection::Connection()
   : _local_endpoint()
   , _remote_endpoint()
   , _read_deadline()
   , _write_deadline()
{
}

Connection::~Connection()
{
}

EndPoint* Connection::local_endpoint() const
{
   return _local_endpoint.get();
}

EndPoint* Connection::remote_endpoint() const
{
   return _remote_endpoint.get();
}

std::error_code Connection::deadline(const std::chrono::seconds& sec)
{
   _read_deadline  = sec;
   _write_deadline = sec;
   return std::error_code();
}

std::chrono::seconds Connection::deadline() const
{
   return _read_deadline;
}

std::error_code Connection::read_deadline(const std::chrono::seconds& sec)
{
   _read_deadline = sec;
   return std::error_code();
}

std::chrono::seconds Connection::read_deadline() const
{
   return _read_deadline;
}

std::error_code Connection::write_deadline(const std::chrono::seconds& sec)
{
   _write_deadline = sec;
   return std::error_code();
}

std::chrono::seconds Connection::write_deadline() const
{
   return _write_deadline;
}

} // net
} // orion
