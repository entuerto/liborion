//
// Error.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_HOST_WIN32_ERROR_H
#define ORION_HOST_WIN32_ERROR_H

#include <orion/Orion-Stddefs.h>

#include <host/win32/String.h>

#include <system_error>

namespace orion
{
namespace win32
{
///
/// Error codes for win32 functions.
///
enum class ErrorCode
{
};

//--------------------------------------------------------------------------------------------------

class ErrorCodeCategory : public std::error_category
{
public:
   const char* name() const noexcept override
   {
      return "win32 errors";
   }

   std::string message(int err_code) const override
   {
      std::string error_message;

      win32::format_error_message(static_cast<uint32_t>(err_code), error_message);
   
      return error_message;
   }
};

//--------------------------------------------------------------------------------------------------

inline const std::error_category& get_error_category()
{
   static ErrorCodeCategory res;
   return res;
}

//--------------------------------------------------------------------------------------------------

///
/// Make an error code object
///
inline std::error_code make_error_code(uint32_t code) noexcept
{
   return {static_cast<int>(code), get_error_category()};
}

///
/// Make an error code object
///
inline std::error_code make_error_code(ErrorCode code) noexcept
{
   return {static_cast<int>(code), get_error_category()};
}

///
/// Make an error condition object
///
inline std::error_condition make_error_condition(ErrorCode code) noexcept
{
   return {static_cast<int>(code), get_error_category()};
}

} // namespace win32
} // namespace orion

namespace std
{
template<>
struct is_error_condition_enum<orion::win32::ErrorCode> : true_type
{
};
} // namespace std

#endif // ORION_HOST_WIN32_ERROR_H
