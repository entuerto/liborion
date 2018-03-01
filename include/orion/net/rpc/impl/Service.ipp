//
// Service.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_RPC_SERVICE_IPP
#define ORION_RPC_SERVICE_IPP

#include <orion/Orion-Stddefs.h>

namespace orion
{
namespace net
{
namespace rpc
{

//-------------------------------------------------------------------------------------------------

template<typename Protocol>
Service<Protocol>::Service(const std::string& name, const Version& version)
   : _name(name)
   , _description()
   , _version(version)
   , _methods()
{
}

template<typename Protocol>
Service<Protocol>::Service(const std::string& name, const Version& version, const std::string& desc)
   : _name(name)
   , _description(desc)
   , _version(version)
   , _methods()
{
}

template<typename Protocol>
std::string Service<Protocol>::name() const
{
   return _name;
}

template<typename Protocol>
std::string Service<Protocol>::description() const
{
   return _description;
}

template<typename Protocol>
Version Service<Protocol>::version() const
{
   return _version;
}

template<typename Protocol>
template<typename MethodT>
void Service<Protocol>::remove_method()
{
   auto it = _methods.find(MethodT::name());

   if (it != _methods.end())
      _methods.erase(it);
}

template<typename Protocol>
void Service<Protocol>::clear_methods()
{
   _methods.clear();
}

template<typename Protocol>
void Service<Protocol>::dump() const
{
   static const std::string service_text = R"(
Service

Name        : {}
Description : {}
Version     : {}

Methods:)";

   static const std::string method_text = R"(
Name        : {}
Description : {}
Version     : {}
Prototype   : {})";

   log::write(fmt::format(service_text, _name, _description, _version));

   for (const auto& info : _method_info)
   {
      log::write(
         fmt::format(method_text, info.name, info.description, info.version, info.prototype));
   }
   log::write();
}

//-------------------------------------------------------------------------------------------------

} // namespace rpc
} // namespace net
} // namespaceorion
#endif // ORION_RPC_SERVICE_IPP