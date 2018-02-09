//
// Listener.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_LISTENER_H
#define ORION_NET_LISTENER_H

#include <orion/Orion-Stddefs.h>

#include <system_error>

namespace orion
{
namespace net
{

///
/// Accepts incoming connections. 
///
class Listener 
{
public:
   virtual ~Listener() = default;

   /// Endpoint where it will accepts incoming connections. 
   virtual EndPoint endpoint() const =0;

   /// Indicates if we are still listening for incoming connections.
   virtual bool is_listening() const =0;

   /// Start accepting incoming connections.
   virtual std::error_code start() =0;

   /// Close closes the listener.
   virtual std::error_code close() =0;
};

} // namespace net
} // namespace orion

#endif // ORION_NET_LISTENER_H
