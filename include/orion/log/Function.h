// Function.h
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

#ifndef ORION_LOG_FUNCTION_H
#define ORION_LOG_FUNCTION_H

#include <orion/Orion-Stddefs.h>

#include <orion/SourceLocation.h>
#include <orion/log/Level.h>
#include <string>

namespace orion
{
namespace log
{
//! A Function represents an function being logged.
/*!
 */
class API_EXPORT Function
{
public:
   Function(Level level, const SourceLocation& src_loc);

   Function(const Function& rhs);
   Function(Function&& rhs);

   virtual ~Function();

   //! Returns the level of the log record
   virtual Level level() const;

   //! Records the level of the log record
   virtual void level(Level level);

   //! Returns the function name recorded
   virtual std::string name() const;

   //! Records a function name
   virtual void name(const std::string& value);

   Function& operator=(const Function& rhs);
   Function& operator=(Function&& rhs);

private:
   Level _level;
   std::string _name;
};

} // namespace log
} // namespace orion

#endif /* ORION_LOG_FUNCTION_H */
