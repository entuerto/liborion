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

RpcError::RpcError(const RpcError& Other) :
   _code(Other._code),
   _message(Other._message),
   _data(Other._data)
{
}

RpcError::RpcError(RpcError&& Other) :
   _code(std::move(Other._code)),
   _message(std::move(Other._message)),
   _data(std::move(Other._data))
{
}

RpcError::~RpcError()
{
}

RpcError& RpcError::operator=(const RpcError& Rhs)
{
   if (this == &Rhs)
      return *this;

   _code = Rhs._code;
   _message = Rhs._message;
   _data = Rhs._data;

   return *this;
}

RpcError& RpcError::operator=(RpcError&& Rhs)
{
   _code = std::move(Rhs._code);
   _message = std::move(Rhs._message);
   _data = std::move(Rhs._data);

   return *this;
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
std::unique_ptr<RpcError> RpcError::create(int32_t code, const std::string& message, const std::string& data)
{
   return std::make_unique<RpcError>(code, message, data);
}

} // ws
} // orion

