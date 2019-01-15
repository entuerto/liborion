//
// Utils.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/tcp/Utils.h>

#include <asio.hpp>

namespace orion
{
namespace net
{
namespace tcp
{

std::unique_ptr<std::streambuf> make_buffer(std::size_t max_size /* = 0 */)
{
   if (max_size != 0)
      return std::make_unique<asio::streambuf>(max_size);

   return std::make_unique<asio::streambuf>();
}

//-------------------------------------------------------------------------------------------------

Handler::State Handler::state() const
{
   return _state;
}

void Handler::state(Handler::State value)
{
   _state = value;
}

bool Handler::read_wanted() const 
{ 
   return _state == State::Read; 
}

bool Handler::write_wanted() const 
{ 
   return _state == State::Write; 
}

bool Handler::should_stop() const  
{ 
   return _state == State::Close; 
}

void Handler::signal_write()
{
   _state = State::Write;
}

} // tcp
} // net
} // orion
