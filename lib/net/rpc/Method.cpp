//
// Method.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/rpc/Method.h>

namespace orion
{
namespace net
{
namespace rpc
{
   
Method::Method(const std::string& name, const std::string& desc) :
   _name(name),
   _description(desc)
{
}

Method::Method(const Method& rhs) :
   _name(rhs._name),
   _description(rhs._description)
{
}

Method::Method(Method&& rhs) :
   _name(std::move(rhs._name)),
   _description(std::move(rhs._description))
{
}

std::string Method::name() const
{
   return _name;
}

std::string Method::description() const
{
   return _description;
}

Method& Method::operator=(const Method& rhs)
{
   if (this == &rhs)
      return *this;

   _name        = rhs._name;
   _description = rhs._description;

   return *this;
}

Method& Method::operator=(Method&& rhs)
{
   _name        = std::move(rhs._name);
   _description = std::move(rhs._description);

   return *this;
}

} // namespace rpc
} // namespace net
} // namespace orion
