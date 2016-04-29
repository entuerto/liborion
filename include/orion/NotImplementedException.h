// NotImplementedException.h
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

#ifndef ORION_NOTIMPLEMENTEDEXCEPTION_H
#define ORION_NOTIMPLEMENTEDEXCEPTION_H

#include <cstdint>
#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/Exception.h>

namespace orion
{
//! Not implemented exception
/*!
    The exception that is thrown when a requested method
    or operation is not implemented.
 */
class API_EXPORT NotImplementedException : public Exception
{
public:
   NotImplementedException(const std::string& text);
   NotImplementedException(const std::string& text,
                           const std::string& file_name,
                                 int32_t      line_number);
   NotImplementedException(const std::string& text,
                           const std::string& file_name,
                                 int32_t      line_number,
                           const std::string& function);
   NotImplementedException(const NotImplementedException& other);
   virtual ~NotImplementedException() throw();

   NotImplementedException& operator=(const NotImplementedException& other);

};

} // namespace orion
#endif /* ORION_NOTIMPLEMENTEDEXCEPTION_H */
