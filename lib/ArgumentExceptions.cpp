// ArgumentExceptions.cpp
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

#include <orion/ArgumentExceptions.h>

namespace orion
{
// ---------------------------------------------------------------------------
// ---- ArgumentException ----

ArgumentException::ArgumentException(const std::string& text) :
   Exception(text)
{
}

ArgumentException::ArgumentException(const std::string& text,
                                     const std::string& file_name,
                                           int32_t      line_number) :
   Exception(text, file_name, line_number)
{
}

ArgumentException::ArgumentException(const std::string& text,
                                     const std::string& file_name,
                                           int32_t      line_number,
                                     const std::string& function) :
   Exception(text, file_name, line_number, function)
{
}

ArgumentException::ArgumentException(const ArgumentException& other) :
   Exception(other)
{
}

ArgumentException::ArgumentException(const ArgumentException&& other) :
   Exception(std::move(other))
{
}

ArgumentException::~ArgumentException() throw()
{
}

ArgumentException& ArgumentException::operator=(const ArgumentException& other)
{
   if (this == &other) {
      return *this;
   }

   Exception::operator=(other);

   return *this ;
}

ArgumentException& ArgumentException::operator=(const ArgumentException&& other)
{
   Exception::operator=(std::move(other));

   return *this ;
}

// ---------------------------------------------------------------------------
// ---- ArgumentNullException ----

ArgumentNullException::ArgumentNullException(const std::string& text) :
   ArgumentException(text)
{
}

ArgumentNullException::ArgumentNullException(const std::string& text,
                                             const std::string& file_name,
                                                   int32_t      line_number) :
   ArgumentException(text, file_name, line_number)
{
}

ArgumentNullException::ArgumentNullException(const std::string& text,
                                             const std::string& file_name,
                                                   int32_t      line_number,
                                             const std::string& function) :
   ArgumentException(text, file_name, line_number, function)
{
}

ArgumentNullException::ArgumentNullException(const ArgumentNullException& other) :
   ArgumentException(other)
{
}

ArgumentNullException::ArgumentNullException(const ArgumentNullException&& other) :
   ArgumentException(std::move(other))
{
}

ArgumentNullException::~ArgumentNullException() throw()
{
}

ArgumentNullException& ArgumentNullException::operator=(const ArgumentNullException& other)
{
   if (this == &other) {
      return *this;
   }

   ArgumentException::operator=(other);

   return *this ;
}

ArgumentNullException& ArgumentNullException::operator=(const ArgumentNullException&& other)
{
   ArgumentException::operator=(std::move(other));

   return *this ;
}

// ---------------------------------------------------------------------------
// ---- ArgumentOutOfRangeException ----

ArgumentOutOfRangeException::ArgumentOutOfRangeException(const std::string& text) :
   ArgumentException(text)
{
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(const std::string& text,
                                                         const std::string& file_name,
                                                               int32_t      line_number) :
   ArgumentException(text, file_name, line_number)
{
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(const std::string& text,
                                                         const std::string& file_name,
                                                               int32_t      line_number,
                                                         const std::string& function) :
   ArgumentException(text, file_name, line_number, function)
{
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(const ArgumentOutOfRangeException& other) :
   ArgumentException(other)
{
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(const ArgumentOutOfRangeException&& other) :
   ArgumentException(std::move(other))
{
}

ArgumentOutOfRangeException::~ArgumentOutOfRangeException() throw()
{
}

ArgumentOutOfRangeException& ArgumentOutOfRangeException::operator=(const ArgumentOutOfRangeException& other)
{
   if (this == &other) {
      return *this;
   }

   ArgumentException::operator=(other);

   return *this ;
}

ArgumentOutOfRangeException& ArgumentOutOfRangeException::operator=(const ArgumentOutOfRangeException&& other)
{
   ArgumentException::operator=(std::move(other));

   return *this ;
}

} // namespace orion
