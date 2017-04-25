//
// Connection.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <orion/net/tcp/Connection.h>

namespace orion
{
namespace net
{
namespace tcp
{
//---------------------------------------------------------------------------------------
// Connection

Connection::Connection():
   net::Connection(),
   _keep_alive(true),
   _no_delay(true)
{
}

Connection::~Connection()
{
}

std::error_code Connection::keep_alive(bool value)
{
   _keep_alive = value;
   return std::error_code();
}

bool Connection::keep_alive() const
{
   return _keep_alive;
}

std::error_code Connection::no_delay(bool value)
{
   _no_delay = value;
   return std::error_code();
}

bool Connection::no_delay() const
{
   return _no_delay;
}

} // tcp
} // net
} // orion
