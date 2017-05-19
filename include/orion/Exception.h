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

#include <orion/Orion-Stddefs.h>
#include <orion/SourceLocation.h>

#include <cstdint>
#include <string>

namespace orion
{
//! Generic exception
/*!
    Generic exception that all library exceptions inherits.
 */
class API_EXPORT Exception : public std::exception
{
public:
   Exception(const std::string& text, const SourceLocation& src_loc = SourceLocation{});

   Exception(const Exception& other);
   Exception(Exception&& other);

   virtual ~Exception() = default;

   //! Returns the recorded source location
   virtual const SourceLocation& source_location() const;

   const char* what() const override;

   Exception& operator=(const Exception& other);
   Exception& operator=(Exception&& other);

private:
   std::string _text;

   SourceLocation _src_location;
};

} // namespace orion
#endif /* ORION_EXCEPTION_H */
