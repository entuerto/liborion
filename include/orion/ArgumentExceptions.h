// ArgumentExceptions.h
//
// Copyright 2013 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#ifndef ORION_ARGUMENTEXCEPTIONS_H
#define ORION_ARGUMENTEXCEPTIONS_H

#include <cstdint>
#include <string>

#include <orion/Exception.h>

#ifndef THROW_IF_NULL
#define THROW_IF_NULL(value) \
   if ((value) == NULL) { \
      throw ArgumentNullException("ArgumentNullException: " #value " is NULL", __FILE__, __LINE__, __PRETTY_FUNCTION__); \
   }
#endif

namespace orion
{
//! Argument exception
/*!
    The exception that is thrown when one of the arguments
    provided to a method is not valid.
 */
class ArgumentException : public Exception
{
public:
   ArgumentException(const std::string& text);
   ArgumentException(const std::string& text,
                     const std::string& file_name,
                           int32_t      line_number);
   ArgumentException(const std::string& text,
                     const std::string& file_name,
                           int32_t      line_number,
                     const std::string& function);
   ArgumentException(const ArgumentException& other);
   virtual ~ArgumentException() throw();

   ArgumentException& operator=(const ArgumentException& other);
};

//! Argument null exception
/*!
    The exception that is thrown when a null pointer is passed to a
    method that does not accept it as a valid argument.
 */
class ArgumentNullException : public ArgumentException
{
public:
   ArgumentNullException(const std::string& text);
   ArgumentNullException(const std::string& text,
                         const std::string& file_name,
                               int32_t      line_number);
   ArgumentNullException(const std::string& text,
                         const std::string& file_name,
                               int32_t      line_number,
                         const std::string& function);
   ArgumentNullException(const ArgumentNullException& other);
   virtual ~ArgumentNullException() throw();

   ArgumentNullException& operator=(const ArgumentNullException& other);
};

//! Argument out of range exception
/*!
    The exception that is thrown when the value of an argument is
    outside the allowable range of values as defined by the invoked
    method.
 */
class ArgumentOutOfRangeException : public ArgumentException
{
public:
   ArgumentOutOfRangeException(const std::string& text);
   ArgumentOutOfRangeException(const std::string& text,
                               const std::string& file_name,
                                     int32_t      line_number);
   ArgumentOutOfRangeException(const std::string& text,
                               const std::string& file_name,
                                     int32_t      line_number,
                               const std::string& function);
   ArgumentOutOfRangeException(const ArgumentOutOfRangeException& other);
   virtual ~ArgumentOutOfRangeException() throw();

   ArgumentOutOfRangeException& operator=(const ArgumentOutOfRangeException& other);
};

} // namespace orion
#endif /* ORION_ARGUMENTEXCEPTIONS_H */
