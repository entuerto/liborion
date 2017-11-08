//
// Service.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/rpc/Service.h>

#include <orion/ErrorMacros.h>
#include <orion/Log.h>

namespace orion
{
namespace net
{
namespace rpc
{

//-------------------------------------------------------------------------------------------------
/*
Service::Service(const std::string& name,
                 const std::string& desc,
                 float version,
                 const std::string& url)
   : _name(name)
   , _description(desc)
   , _version(version)
   , _url(url)
   , _methods()
{
}

std::string Service::name() const
{
   return _name;
}

std::string Service::description() const
{
   return _description;
}

float Service::version() const
{
   return _version;
}

std::string Service::url() const
{
   return _url;
}

std::weak_ptr<Method> Service::get_method(const std::string& name) const
{
   const auto& it = _methods.find(name);

   if (it != _methods.end())
   {
      return it->second;
   }

   return std::weak_ptr<Method>();
}
*/
} // namespace rpc
} // namespace net
} // namespace orion
