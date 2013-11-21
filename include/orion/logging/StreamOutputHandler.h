/*
 * StreamOutputHandler.h
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

#ifndef ORION_LOGGING_STREAMOUTPUTHANDLER_H
#define ORION_LOGGING_STREAMOUTPUTHANDLER_H

#include <iosfwd>

#include <orion/NonCopyable.h>
#include <orion/logging/OutputHandler.h>

namespace orion
{
namespace logging
{
class IFormatter;
class LogRecord;

//--------------------------------------------------------------------------
// Class StreamOutputHandler

//! Output the log records to a stream
/*!
    An output handler class which writes logging records formatted by the specified
    formatter to a stream.

    Note that this class does not close the stream, as cout or cerr may be used.
 */
class StreamOutputHandler :
   public OutputHandler,
   NonCopyable
{
public:
   StreamOutputHandler(std::ostream& stream);
   virtual ~StreamOutputHandler();

   virtual void write(const LogRecord& log_record);

   virtual void flush();

   virtual void close();

   static OutputHandler::SharedPtr create(std::ostream& stream);

private:
   std::ostream& _ostream;
};

} // namespace logging
} // namespace orion
#endif /* ORION_LOGGING_STREAMOUTPUTHANDLER_H */
