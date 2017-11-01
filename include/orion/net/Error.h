//
// Error.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_ERROR_H
#define ORION_NET_ERROR_H

#include <orion/Orion-Stddefs.h>

#include <string>
#include <system_error>

namespace orion
{
namespace net
{
///
///
///
enum class ErrorCode
{
   InvalidAddress,
   InvalidAddressV4,
   InvalidAddressV6
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
const std::error_category& get_error_category();

///
///
///
std::error_code make_error_code(ErrorCode code) noexcept;

///
///
///
std::error_condition make_error_condition(ErrorCode code) noexcept;

} // namespace net
} // namespace orion

namespace std
{
template<>
struct is_error_condition_enum<orion::net::ErrorCode> : true_type
{
};
}

#endif // ORION_NET_ERROR_H
