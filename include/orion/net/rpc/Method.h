/*
 * RpcMethod.h
 *
 * Copyright 2013 tomas <tomasp@videotron.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef ORION_NET_RPC_METHOD_H
#define ORION_NET_RPC_METHOD_H

#include <orion/Orion-Stddefs.h>

namespace orion
{
namespace net
{
namespace rpc
{
template <typename T>
class Method;
///
/// Class to define RPC methods
///
template <typename Ret, typename... Args>
class Method<Ret(Args...)>
{
public:
   /// User defined function type.
   using Type = Ret(Args...);
   /// Return type.
   using ReturnType = Ret;
};

} // namespace rpc
} // namespace net
} // namespace orion

#endif // ORION_NET_RPC_METHOD_H
