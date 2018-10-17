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
   explicit Exception(std::string text);
   Exception(std::string text, SourceLocation src_loc);

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
// -- InvalidArgumentError 

/// Invalid argument error
///
/// The error is thrown when one of the arguments provided to a method is not valid.
///
class InvalidArgumentError : public Exception
{
public:
   explicit InvalidArgumentError(std::string text)
      : Exception(std::move(text))
   {
   }
   InvalidArgumentError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

// ------------------------------------------------------------------------------------------------
// -- NullArgumentError

/// Null argument error
///
/// The error is thrown when a null pointer is passed to a method that does not accept 
/// it as a valid argument.
///
class NullArgumentError : public Exception
{
public:
   explicit NullArgumentError(std::string text)
      : Exception(std::move(text))
   {
   }
   NullArgumentError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

// ------------------------------------------------------------------------------------------------
// -- OutOfRangeError 

/// Out of range error
///
/// It reports errors that are consequence of attempt to access elements out of defined range.
///
class OutOfRangeError : public Exception
{
public:
   explicit OutOfRangeError(std::string text)
      : Exception(std::move(text))
   {
   }
   OutOfRangeError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

// ------------------------------------------------------------------------------------------------
// -- NotImplementedError 

/// Not implemented error
///
/// The error is thrown when a requested method or operation is not implemented.
///
class NotImplementedError : public Exception
{
public:
   explicit NotImplementedError(std::string text)
      : Exception(std::move(text))
   {
   }
   NotImplementedError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

// ------------------------------------------------------------------------------------------------
// -- SymbolNotFoundError 

/// Symbol not found error
///
/// This error is thrown when we cannot find a symbol in a module.
///
class SymbolNotFoundError : public Exception
{
public:
   explicit SymbolNotFoundError(std::string text)
      : Exception(std::move(text))
   {
   }
   SymbolNotFoundError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

} // namespace orion

#include <orion/impl/Exception.ipp>

#endif /* ORION_EXCEPTION_H */
