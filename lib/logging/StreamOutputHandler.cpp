/*
 * StreamOutputHandler.cpp
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

#include <iostream>

#include <orion/logging/IFormatter.h>
#include <orion/logging/StreamOutputHandler.h>

namespace orion
{
namespace logging
{
//--------------------------------------------------------------------------
// Class StreamOutputHandler

StreamOutputHandler::StreamOutputHandler(std::ostream& stream) :
   _ostream(stream)
{
}

StreamOutputHandler::~StreamOutputHandler()
{
}

void StreamOutputHandler::write(const LogRecord& log_record)
{
   _ostream << formatter()->format(log_record)
            << std::endl;
}

void StreamOutputHandler::flush()
{
   _ostream << std::flush;;
}

void StreamOutputHandler::close()
{
   // Do not close stream in case cout or cerr is used.
}

OutputHandler::SharedPtr StreamOutputHandler::create(std::ostream& stream)
{
   return OutputHandler::SharedPtr(new StreamOutputHandler(stream));
}

} // namespace logging
} // namespace orion
