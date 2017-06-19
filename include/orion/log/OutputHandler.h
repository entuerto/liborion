/*
 * OutputHandler.h
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

#ifndef ORION_LOG_OUTPUTHANDLER_H
#define ORION_LOG_OUTPUTHANDLER_H

#include <orion/Orion-Stddefs.h>
#include <orion/log/Formatter.h>

#include <memory>
#include <vector>

namespace orion
{
namespace log
{
class Record;

//--------------------------------------------------------------------------
// Class OutputHandler

//! OutputHandler instances dispatch logging events to specific destinations.
/*!
    Output handlers can optionally use Formatter instances to format
    records as desired.

    By default, a basic one line formatter is specified.
 */
class API_EXPORT OutputHandler
{
public:
   OutputHandler();
   virtual ~OutputHandler();

   //! Gets the formatter of the output handler
   Formatter* formatter() const;

   //! Sets the formatter of the output handler.
   void set_formatter(std::unique_ptr<Formatter>&& formatter);

   //! Writes a log record
   virtual void write(const Record& record) = 0;

   //! Flushes the stream
   virtual void flush() = 0;

   //! Closes the output handler
   virtual void close() = 0;

private:
   std::unique_ptr<Formatter> _formatter;
};

typedef std::vector<std::unique_ptr<OutputHandler>> OutputHandlers;

} // namespace log
} // namespace orion

#endif /* ORION_LOG_OUTPUTHANDLER_H */
