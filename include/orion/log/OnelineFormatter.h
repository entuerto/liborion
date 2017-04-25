// OnelineFormatter.h
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

#ifndef ORION_LOG_ONELINEFORMATTER_H
#define ORION_LOG_ONELINEFORMATTER_H

#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/log/Formatter.h>

namespace orion
{
namespace log
{
class Record;

//! One line log formatter
/*!
    Formats the log record on a single line.
 */
class API_EXPORT OnelineFormatter : public Formatter
{
public:
   OnelineFormatter();
   virtual ~OnelineFormatter();

   virtual std::string format(const Record& record);
};

} // namespace log
} // namespace orion
#endif /* ORION_LOG_ONELINEFORMATTER_H */