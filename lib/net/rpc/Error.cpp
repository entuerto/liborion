//
// Error.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/rpc/Error.h>

namespace orion
{
namespace net
{
namespace rpc
{

Error::Error()
   : _code(0)
   , _message()
   , _data()
{
}

Error::Error(int32_t code)
   : _code(code)
   , _message()
   , _data()
{
}

Error::Error(int32_t code, const std::string& message, const std::string& data /* = "" */)
   : _code(code)
   , _message(message)
   , _data(data)
{
}

Error::Error(const Error& Other)
   : _code(Other._code)
   , _message(Other._message)
   , _data(Other._data)
{
}

Error::Error(Error&& Other)
   : _code(std::move(Other._code))
   , _message(std::move(Other._message))
   , _data(std::move(Other._data))
{
}

Error::~Error()
{
}

Error& Error::operator=(const Error& Rhs)
{
   if (this == &Rhs)
      return *this;

   _code    = Rhs._code;
   _message = Rhs._message;
   _data    = Rhs._data;

   return *this;
}

Error& Error::operator=(Error&& Rhs)
{
   _code    = std::move(Rhs._code);
   _message = std::move(Rhs._message);
   _data    = std::move(Rhs._data);

   return *this;
}

Error::operator bool() const
{
   return _code != 0;
}

//! A Number that indicates the error type that occurred.
int32_t Error::code() const
{
   return _code;
}

//! A String providing a short description of the error.
std::string Error::message() const
{
   return _message;
}

//! A Primitive or Structured value that contains additional information about the error.
std::string Error::data() const
{
   return _data;
}

} // namespace rpc
} // namespace net
} // namespace orion
