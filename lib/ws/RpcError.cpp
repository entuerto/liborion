/*
 * RpcError.cpp
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

#include <orion/ws/RpcError.h>

namespace orion
{
namespace ws
{

RpcError::RpcError(int32_t code, const std::string& message, const std::string& data) :
   _code(code),
   _message(message),
   _data(data)
{

}

RpcError::~RpcError()
{
}

//! A Number that indicates the error type that occurred.
int32_t RpcError::code() const
{
   return _code;
}

//! A String providing a short description of the error.
std::string RpcError::message() const
{
   return _message;
}

//! A Primitive or Structured value that contains additional information about the error.
std::string RpcError::data() const
{
   return _data;
}


//! Create a RPC Error object
RpcError::SharedPtr RpcError::create(int32_t code, const std::string& message, const std::string& data)
{
   return RpcError::SharedPtr(new RpcError(code, message, data));
}

} // ws
} // orion

