/*
 * log-outputhandlers.h
 *
 * Copyright 2013 tomas <tomasp@videotron.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef ORION_LOGGING_OUTPUTHANDLER_H
#define ORION_LOGGING_OUTPUTHANDLER_H

#include <iosfwd>
#include <vector>

#include <orion/MemoryUtils.h>
#include <orion/logging/IFormatter.h>

namespace orion
{
namespace logging
{
class LogRecord;

//--------------------------------------------------------------------------
// Class OutputHandler

//! OutputHandler instances dispatch logging events to specific destinations.
/*!
    Output handlers can optionally use Formatter instances to format
    records as desired. By default, a basic one line formatter is specified.
 */
class OutputHandler 
{
public:
   DECLARE_POINTERS(OutputHandler)

   typedef std::vector<SharedPtr> SharedPtrVector;

   OutputHandler();
   virtual ~OutputHandler();

   //! Gets the formatter of the output handler
   IFormatter::SharedPtr formatter() const;

   //! Sets the formatter of the output handler
   void set_formatter(IFormatter::SharedPtr formatter);

   //! Writes a log record
   virtual void write(const LogRecord& log_record) =0;

   //! Flushes the stream
   virtual void flush() =0;

   //! Closes the output handler
   virtual void close() =0;

private:
   IFormatter::SharedPtr _formatter;

};

} // namespace logging
} // namespace orion

#endif /* ORION_LOGGING_OUTPUTHANDLER_H */
