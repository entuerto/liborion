//
// Utils.h
//
//  Created by Tomas Palazuelos on 2017-10-13.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_TCP_UTILS_H
#define ORION_NET_TCP_UTILS_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/Utils.h>

#include <functional>
#include <memory>
#include <streambuf>

namespace orion
{
namespace net
{
namespace tcp
{
//-------------------------------------------------------------------------------------------------

using Handler = std::function<std::error_code(std::streambuf*, std::streambuf*)>;

using ConnectHandler = std::function<void(const std::error_code&)>;
using AcceptHandler  = std::function<void(const std::error_code&)>;
using ReadHandler    = std::function<void(const std::error_code&, std::streambuf*)>;
using WriteHandler   = std::function<void(const std::error_code&, std::size_t)>;

std::unique_ptr<std::streambuf> make_buffer(std::size_t max_size = 0);

//-------------------------------------------------------------------------------------------------
// Tcp Options
using NoDelay = Option<0, bool>;

} // tcp
} // net
} // orion
#endif // ORION_NET_TCP_UTILS_H
