/*
 * RpcError.h
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

#ifndef ORION_WS_RPCERROR_H
#define ORION_WS_RPCERROR_H

#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>

namespace orion
{
namespace ws
{
//! Abstract class to define RPC errors
/*!
   
 */
class API_EXPORT RpcError
{
public:
   RpcError(int32_t code, const std::string& message, const std::string& data);

   RpcError(const RpcError& Other);
   RpcError(RpcError&& Other);

   virtual ~RpcError();

   RpcError& operator=(const RpcError& Rhs);
   RpcError& operator=(RpcError&& Rhs);

   //! A Number that indicates the error type that occurred.
   virtual int32_t code() const;

   //! A String providing a short description of the error.
   virtual std::string message() const;

   //! A Primitive or Structured value that contains additional information about the error.
   virtual std::string data() const;

public:
   //! Create a RPC Error object
   static std::unique_ptr<RpcError> create(int32_t code, const std::string& message, const std::string& data = "");

private:
  int32_t _code; 
  std::string _message; 
  std::string _data;
};

} // ws
} // orion

#endif

