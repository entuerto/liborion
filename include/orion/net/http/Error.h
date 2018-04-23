//
// Error.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_ERROR_H
#define ORION_NET_HTTP_ERROR_H

#include <orion/Orion-Stddefs.h>

#include <string>
#include <system_error>

namespace orion
{
namespace net
{
namespace http
{
///
///
///
enum class ErrorCode
{
   BadRequest,
   Unauthorized,
   NotFound,
   
   FieldTextNotFound,
   MethodTextNotFound
};

///
///
///
class ErrorCodeCategory : public std::error_category
{
public:
   const char* name() const noexcept override;

   std::string message(int err_code) const override;
};

///
///
///
inline const std::error_category& get_error_category()
{
   static ErrorCodeCategory res;
   return res;
}

///
///
///
inline std::error_code make_error_code(ErrorCode code) noexcept
{
   return {static_cast<int>(code), get_error_category()};
}

///
///
///
inline std::error_condition make_error_condition(ErrorCode code) noexcept
{
   return {static_cast<int>(code), get_error_category()};
}

} // namespace http
} // namespace net
} // namespace orion

namespace std
{
template<>
struct is_error_condition_enum<orion::net::http::ErrorCode> : true_type
{
};
} // namespace std

#endif // ORION_NET_HTTP_ERROR_H
