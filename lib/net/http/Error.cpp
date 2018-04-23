//
// Error.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/http/Error.h>

#include <map>

namespace orion
{
namespace net
{
namespace http
{
static const std::map<ErrorCode, std::string> ErrorText{
   {ErrorCode::BadRequest, "Bad Request"},
   {ErrorCode::Unauthorized, "Unauthorized"},
   {ErrorCode::NotFound, "Not Found"},
   {ErrorCode::FieldTextNotFound, "Field text not found"},
   {ErrorCode::MethodTextNotFound, "Method text not found"}};

const char* ErrorCodeCategory::name() const noexcept
{
   return "liborion-net http errors";
}

std::string ErrorCodeCategory::message(int err_code) const
{
   auto text = ErrorText.find(static_cast<ErrorCode>(err_code));
   if (text != ErrorText.end())
      return text->second;

   return "Unknown code";
}

} // namespace http
} // namespace net
} // namespace orion
