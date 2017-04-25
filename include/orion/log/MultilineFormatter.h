// MultilineFormatter.h
//
// Copyright 2009 tomas <tomasp@videotron.ca>
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

#ifndef ORION_LOG_MULTILINEFORMATTER_H
#define ORION_LOG_MULTILINEFORMATTER_H

#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/SourceLocation.h>
#include <orion/log/Formatter.h>

namespace orion
{
namespace log
{
class Record;

//! Multi line formatter
/*!
    Formats the information on more than one line.
 */
class API_EXPORT MultilineFormatter : public Formatter
{
public:
   MultilineFormatter();
   virtual ~MultilineFormatter();

   virtual std::string format(const Record& record);

private:
   std::string format_source_info(const std::string& prefix, const SourceLocation& src_loc);

};

} // namespace log
} // namespace orion
#endif /* ORION_LOG_MULTILINEFORMATTER_H */
