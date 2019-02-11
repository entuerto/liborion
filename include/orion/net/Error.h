//
// Error.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_ERROR_H
#define ORION_NET_ERROR_H

#include <orion/Config.h>

#include <system_error>

namespace orion
{
namespace net
{
///
/// Error codes for the networking namespace.
///
enum class ErrorCode
{
   InvalidAddress,
   InvalidAddressV4,
   InvalidAddressV6
};

///
/// Make an error code object
///
API_EXPORT std::error_code make_error_code(ErrorCode code) noexcept;

///
/// Make an error condition object
///
API_EXPORT std::error_condition make_error_condition(ErrorCode code) noexcept;

} // namespace net
} // namespace orion

namespace std
{
template<>
struct is_error_condition_enum<orion::net::ErrorCode> : true_type
{
};
} // namespace std

#endif // ORION_NET_ERROR_H
