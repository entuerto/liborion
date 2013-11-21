// Date-utils.h
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

#ifndef ORION_DATE_UTILS_H
#define ORION_DATE_UTILS_H

#include <string>

namespace orion
{
/*!
   Returns the current date representation for current locale.
   \param date  a string to hold the return value.
 */
void get_current_date(std::string& date);

/*!
   Returns the current time representation for current locale.
   \param time  a string to hold the return value.
 */
void get_current_time(std::string& time);

/*!
   Returns the current date and time representation for current locale.
   \param datetime  a string to hold the return value.
 */
void get_current_datetime(std::string& datetime);

} // namespace orion

#endif // ORION_DATE_UTILS_H
