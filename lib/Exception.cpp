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

Exception::Exception(const std::string& text) :
   _line_number(-1),
   _text(text),
   _file_name(),
   _function_name()
{
}

Exception::Exception(const std::string& text,
                     const std::string& file_name,
                           int32_t      line_number) :
   _line_number(line_number),
   _text(text),
   _file_name(file_name),
   _function_name()
{
}

Exception::Exception(const std::string& text,
                     const std::string& file_name,
                           int32_t      line_number,
                     const std::string& function) :
   _line_number(line_number),
   _text(text),
   _file_name(file_name),
   _function_name(function)
{
}

Exception::Exception(const Exception& other) :
   _line_number(other._line_number),
   _text(other._text),
   _file_name(other._file_name),
   _function_name(other._function_name)
{
}

Exception::Exception(const Exception&& other) :
   _line_number(std::move(other._line_number)),
   _text(std::move(other._text)),
   _file_name(std::move(other._file_name)),
   _function_name(std::move(other._function_name))
{
}

Exception::~Exception()  throw()
{
}

std::string Exception::file_name() const
{
   return _file_name;
}

int32_t Exception::line_number() const
{
   return _line_number;
}

std::string Exception::function_name() const
{
   return _function_name;
}

const char* Exception::what() const throw()
{
   return _text.c_str();
}

Exception& Exception::operator=(const Exception& other)
{
   if (this == &other) {
      return *this;
   }

   _text = other._text;
   _file_name = other._file_name;
   _line_number = other._line_number;
   _function_name = other._function_name;

   return *this ;
}

Exception& Exception::operator=(const Exception&& other)
{
   _text = std::move(other._text);
   _file_name = std::move(other._file_name);
   _line_number = std::move(other._line_number);
   _function_name = std::move(other._function_name);

   return *this ;
}

} // namespace orion
