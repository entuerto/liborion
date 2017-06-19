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

ArgumentException::ArgumentException(const std::string& text, const SourceLocation& src_loc)
   : Exception(text, src_loc)
{
}

ArgumentException::ArgumentException(const ArgumentException& other)
   : Exception(other)
{
}

ArgumentException::ArgumentException(ArgumentException&& other)
   : Exception(std::move(other))
{
}

ArgumentException& ArgumentException::operator=(const ArgumentException& other)
{
   if (this == &other)
   {
      return *this;
   }

   Exception::operator=(other);

   return *this;
}

ArgumentException& ArgumentException::operator=(ArgumentException&& other)
{
   Exception::operator=(std::move(other));

   return *this;
}

// ---------------------------------------------------------------------------
// ---- ArgumentNullException ----

ArgumentNullException::ArgumentNullException(const std::string& text, const SourceLocation& src_loc)
   : ArgumentException(text, src_loc)
{
}

ArgumentNullException::ArgumentNullException(const ArgumentNullException& other)
   : ArgumentException(other)
{
}

ArgumentNullException::ArgumentNullException(ArgumentNullException&& other)
   : ArgumentException(std::move(other))
{
}

ArgumentNullException& ArgumentNullException::operator=(const ArgumentNullException& other)
{
   if (this == &other)
   {
      return *this;
   }

   ArgumentException::operator=(other);

   return *this;
}

ArgumentNullException& ArgumentNullException::operator=(ArgumentNullException&& other)
{
   ArgumentException::operator=(std::move(other));

   return *this;
}

// ---------------------------------------------------------------------------
// ---- ArgumentOutOfRangeException ----

ArgumentOutOfRangeException::ArgumentOutOfRangeException(const std::string& text,
                                                         const SourceLocation& src_loc)
   : ArgumentException(text, src_loc)
{
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(const ArgumentOutOfRangeException& other)
   : ArgumentException(other)
{
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(ArgumentOutOfRangeException&& other)
   : ArgumentException(std::move(other))
{
}

ArgumentOutOfRangeException& ArgumentOutOfRangeException::operator=(
   const ArgumentOutOfRangeException& other)
{
   if (this == &other)
   {
      return *this;
   }

   ArgumentException::operator=(other);

   return *this;
}

ArgumentOutOfRangeException& ArgumentOutOfRangeException::operator=(
   ArgumentOutOfRangeException&& other)
{
   ArgumentException::operator=(std::move(other));

   return *this;
}

} // namespace orion
