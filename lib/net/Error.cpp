//
// Error.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/Error.h>

#include <map>

namespace orion
{
namespace net
{
//--------------------------------------------------------------------------------------------------

static const std::map<ErrorCode, std::string> ErrorText{
   {ErrorCode::InvalidAddress,   "Invalid Address"},
   {ErrorCode::InvalidAddressV4, "Invalid IPv4 Address"},
   {ErrorCode::InvalidAddressV6, "Invalid IPv6 Address"}};

//--------------------------------------------------------------------------------------------------

const char* ErrorCodeCategory::name() const noexcept
{
   return "liborion-net errors";
}

std::string ErrorCodeCategory::message(int err_code) const
{
   auto text = ErrorText.find(static_cast<ErrorCode>(err_code));
   if (text != ErrorText.end())
      return text->second;

   return "Unknown code";
}

//--------------------------------------------------------------------------------------------------

const std::error_category& get_error_category()
{
   static ErrorCodeCategory res;
   return res;
}

std::error_code make_error_code(ErrorCode code) noexcept
{
   return std::error_code(static_cast<int>(code), get_error_category());
}

std::error_condition make_error_condition(ErrorCode code) noexcept
{
   return std::error_condition(static_cast<int>(code), get_error_category());
}

} // namespace net
} // namespace orion
