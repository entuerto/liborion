// Exception.cpp
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

#include <orion/Exception.h>

namespace orion
{

Exception::Exception(const std::string& text, const SourceLocation& src_loc) :
   _text(text),
   _src_location(src_loc)
{
}

Exception::Exception(const Exception& other) :
   _text(other._text),
   _src_location(other._src_location)
{
}

Exception::Exception(Exception&& other) :
   _text(std::move(other._text)),
   _src_location(std::move(other._src_location))
{
}

const SourceLocation& Exception::source_location() const
{
   return _src_location;
}

const char* Exception::what() const noexcept
{
   return _text.c_str();
}

Exception& Exception::operator=(const Exception& other)
{
   if (this == &other) {
      return *this;
   }

   _text = other._text;
   _src_location = other._src_location;

   return *this ;
}

Exception& Exception::operator=(Exception&& other)
{
   _text = std::move(other._text);
   _src_location = std::move(other._src_location);

   return *this ;
}

} // namespace orion
