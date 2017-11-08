//
// Error.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_RPC_ERROR_H
#define ORION_NET_RPC_ERROR_H

#include <orion/Orion-Stddefs.h>

#include <memory>
#include <string>

namespace orion
{
namespace net
{
namespace rpc
{
//! Abstract class to define RPC errors
/*!

 */
class API_EXPORT Error
{
public:
   Error();
   Error(int32_t code);
   Error(int32_t code, const std::string& message, const std::string& data = "");

   Error(const Error& Other);
   Error(Error&& Other);

   virtual ~Error();

   Error& operator=(const Error& Rhs);
   Error& operator=(Error&& Rhs);

   explicit operator bool() const;

   //! A Number that indicates the error type that occurred.
   virtual int32_t code() const;

   //! A String providing a short description of the error.
   virtual std::string message() const;

   //! A Primitive or Structured value that contains additional information about the error.
   virtual std::string data() const;

private:
   int32_t _code;
   std::string _message;
   std::string _data;
};

} // namespace rpc
} // namespace net
} // namespace orion

#endif
