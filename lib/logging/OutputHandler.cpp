/*
 * log-outputhandlers.cpp
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

#include <orion/logging/OutputHandler.h>
#include <orion/logging/OnelineFormatter.h>

namespace orion
{
namespace logging
{
//--------------------------------------------------------------------------
// Class OutputHandler

OutputHandler::OutputHandler() :
   _formatter(OnelineFormatter::create())
{
}

OutputHandler::~OutputHandler()
{
}

IFormatter::SharedPtr OutputHandler::formatter() const
{
   return _formatter;
}

void OutputHandler::set_formatter(IFormatter::SharedPtr formatter)
{
   _formatter = formatter;
}

} // namespace logging
} // namespace orion
