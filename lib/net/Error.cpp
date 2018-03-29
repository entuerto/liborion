//
// Error.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/Error.h>

#include <map>
#include <string>

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

///
///
///
class ErrorCodeCategory : public std::error_category
{
public:
   const char* name() const noexcept override
   {
      return "orion-net errors";
   }

   std::string message(int err_code) const override
   {
      auto text = ErrorText.find(static_cast<ErrorCode>(err_code));
      if (text != ErrorText.end())
         return text->second;
   
      return "Unknown code";
   }
};

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
