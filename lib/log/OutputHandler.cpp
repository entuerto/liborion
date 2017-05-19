/*
 * OutputHandler.cpp
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

#include <orion/log/OnelineFormatter.h>
#include <orion/log/OutputHandler.h>

namespace orion
{
namespace log
{
//--------------------------------------------------------------------------
// Class OutputHandler

OutputHandler::OutputHandler()
   : _formatter(std::make_unique<OnelineFormatter>())
{
}

OutputHandler::~OutputHandler()
{
}

Formatter* OutputHandler::formatter() const
{
   return _formatter.get();
}

///
/// the output handler takes ownership of the formatter.
/// @param formatter  Formatter for the output
void OutputHandler::set_formatter(std::unique_ptr<Formatter>&& formatter)
{
   _formatter = std::move(formatter);
}

} // namespace log
} // namespace orion
