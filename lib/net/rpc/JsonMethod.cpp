//
// JsonMethod.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/rpc/JsonMethod.h>

#include <orion/net/rpc/JsonError.h>

namespace orion
{
namespace net
{
namespace rpc
{
   
JsonMethod::JsonMethod(const std::string& name, const std::string& desc) :
   Method(name, desc),
   _func()  
{
}

JsonMethod::JsonMethod(const std::string& name, const std::string& desc, JsonFunctionHandler func) :
   Method(name, desc),
   _func(func) 
{
}


JsonMethod::JsonMethod(const JsonMethod& rhs) :
   Method(rhs),
   _func(rhs._func) 
{
}

JsonMethod::JsonMethod(JsonMethod&& rhs) :
   Method(std::move(rhs)),
   _func(std::move(rhs._func))
{
}

JsonMethod& JsonMethod::operator=(const JsonMethod& rhs)
{
   if (this == &rhs)
      return *this;

   Method::operator=(rhs);

   _func = rhs._func;

   return *this;
}

JsonMethod& JsonMethod::operator=(JsonMethod&& rhs)
{
   Method::operator=(std::move(rhs));

   _func = std::move(rhs._func);

   return *this;
}

Error JsonMethod::operator()(Json::Value& json_request, Json::Value& json_result)
{
   if (not _func)
      return make_error(JsonErrc::InternalError);

   return _func(json_request, json_result);
}

} // namespace rpc
} // namespace net
} // namespace orion
