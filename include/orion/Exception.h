//
// Exception.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_EXCEPTION_H
#define ORION_EXCEPTION_H

#include <orion/Orion-Stddefs.h>
#include <orion/SourceLocation.h>

#include <exception>
#include <string>

namespace orion
{
/// Generic exception
///
/// Generic exception that all library exceptions inherits.
///
class Exception : public std::exception
{
public:
   explicit Exception(std::string text, SourceLocation src_loc = SourceLocation{});

   Exception(const Exception&) = default;
   Exception(Exception&&) noexcept = default;

   virtual ~Exception() = default;

   Exception& operator=(const Exception&) = default;
   Exception& operator=(Exception&&) noexcept = default;

   //! Returns the recorded source location
   virtual const SourceLocation& source_location() const;

   const char* what() const noexcept override;

private:
   std::string _text;

   SourceLocation _src_location;
};

// ------------------------------------------------------------------------------------------------
// -- ArgumentException 

/// Argument exception
///
/// The exception that is thrown when one of the arguments
/// provided to a method is not valid.
///
using ArgumentException = Exception;

// ------------------------------------------------------------------------------------------------
// -- ArgumentNullException 

/// Argument null exception
///
/// The exception that is thrown when a null pointer is passed to a
/// method that does not accept it as a valid argument.
///
using ArgumentNullException = Exception;

// ------------------------------------------------------------------------------------------------
// -- ArgumentOutOfRangeException 

/// Argument out of range exception
///
/// The exception that is thrown when the value of an argument is
/// outside the allowable range of values as defined by the invoked
/// method.
///
using ArgumentOutOfRangeException = Exception;

// ------------------------------------------------------------------------------------------------
// -- NotImplementedException 

/// Not implemented exception
///
/// The exception that is thrown when a requested method
/// or operation is not implemented.
///
using NotImplementedException = Exception;

// ------------------------------------------------------------------------------------------------
// -- ModuleException 

/// General module exception
///
/// Throw this exception for module errors.
///
using ModuleException = Exception;

// ------------------------------------------------------------------------------------------------
// -- ModuleSymbolNotFoundException 

/// Symbol not found exception
///
/// This exception is thrown when loading the modules and we cannot find the symbol in the module.
///
using ModuleSymbolNotFoundException = Exception;

} // namespace orion

#include <orion/impl/Exception.ipp>

#endif /* ORION_EXCEPTION_H */
