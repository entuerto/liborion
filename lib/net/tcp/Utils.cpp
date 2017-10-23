//
// Utils.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
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

} // tcp
} // net
} // orion
