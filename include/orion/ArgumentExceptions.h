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

#include <orion/Orion-Stddefs.h>
#include <orion/Exception.h>
#include <string>

namespace orion
{
//! Argument exception
/*!
    The exception that is thrown when one of the arguments
    provided to a method is not valid.
 */
class API_EXPORT ArgumentException : public Exception
{
public:
   ArgumentException(const std::string& text, const SourceLocation& src_loc = SourceLocation{});

   ArgumentException(const ArgumentException& other);
   ArgumentException(ArgumentException&& other);

   virtual ~ArgumentException() = default;

   ArgumentException& operator=(const ArgumentException& other);
   ArgumentException& operator=(ArgumentException&& other);
};

//! Argument null exception
/*!
    The exception that is thrown when a null pointer is passed to a
    method that does not accept it as a valid argument.
 */
class API_EXPORT ArgumentNullException : public ArgumentException
{
public:
   ArgumentNullException(const std::string& text, const SourceLocation& src_loc = SourceLocation{});

   ArgumentNullException(const ArgumentNullException& other);
   ArgumentNullException(ArgumentNullException&& other);

   virtual ~ArgumentNullException() = default;

   ArgumentNullException& operator=(const ArgumentNullException& other);
   ArgumentNullException& operator=(ArgumentNullException&& other);
};

//! Argument out of range exception
/*!
    The exception that is thrown when the value of an argument is
    outside the allowable range of values as defined by the invoked
    method.
 */
class API_EXPORT ArgumentOutOfRangeException : public ArgumentException
{
public:
   ArgumentOutOfRangeException(const std::string& text,
                               const SourceLocation& src_loc = SourceLocation{});

   ArgumentOutOfRangeException(const ArgumentOutOfRangeException& other);
   ArgumentOutOfRangeException(ArgumentOutOfRangeException&& other);

   virtual ~ArgumentOutOfRangeException() = default;

   ArgumentOutOfRangeException& operator=(const ArgumentOutOfRangeException& other);
   ArgumentOutOfRangeException& operator=(ArgumentOutOfRangeException&& other);
};

} // namespace orion
#endif /* ORION_ARGUMENTEXCEPTIONS_H */
