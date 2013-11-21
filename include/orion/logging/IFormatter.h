// Log-Formatters.h
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

#ifndef ORION_LOGGING_IFORMATTER_H
#define ORION_LOGGING_IFORMATTER_H

#include <string>
#include <orion/MemoryUtils.h>

namespace orion
{
namespace logging
{
class LogRecord;

//! A Formatter is used to convert a LogRecord to text.
/*!
   A Formatter class is responsible for converting a LogRecord to
   a readable form that will be interpreted by either a human or
   an external system.
 */
class IFormatter 
{
public:
   DECLARE_POINTERS(IFormatter)

   virtual ~IFormatter() {}

   //! Formats the LogRecord into a reable form
   virtual std::string format(const LogRecord& log_record) =0;

};

} // namespace logging
} // namespace orion
#endif /* ORION_LOGGING_IFORMATTER_H */
