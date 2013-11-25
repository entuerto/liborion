// Exception.h
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

#ifndef ORION_EXCEPTION_H
#define ORION_EXCEPTION_H

#include <cstdint>
#include <string>

namespace orion
{
//! Generic exception
/*!
    Generic exception that all library exceptions inherits.
 */
class Exception : public std::exception
{
public:
   Exception(const std::string& text);
   Exception(const std::string& text,
             const std::string& file_name,
                   int32_t      line_number);
   Exception(const std::string& text,
             const std::string& file_name,
                   int32_t      line_number,
             const std::string& function);
   Exception(const Exception& other);
   virtual ~Exception() throw();

   std::string file_name() const;

   int32_t line_number() const;

   std::string function_name() const;

   const char* what() const throw();

   Exception& operator=(const Exception& other);

private:
   std::string _text;
   std::string _file_name;
   int32_t _line_number;
   std::string _function_name;
};

} // namespace orion
#endif /* ORION_EXCEPTION_H */
