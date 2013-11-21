// Date-utils.cpp
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

#include <sys/time.h>

#include <orion/DateUtils.h>

namespace orion
{
namespace
{
   void get_current_daytime_generic(std::string& time_str, const char* format)
   {
      struct timeval tv;
      struct tm* ptm;
      // Assumed char* length enough for a datetime for any locale's encoding:
      char time_string[128];
      // Obtain the time of day, and convert it to a tm struct.
      gettimeofday(&tv, NULL);

      ptm = localtime(&tv.tv_sec);

      strftime(time_string, sizeof (time_string), format, ptm);

      time_str = time_string;
   }
}

void get_current_time(std::string& time)
{
   get_current_daytime_generic(time, "%X");
}

void get_current_date(std::string& date)
{
   get_current_daytime_generic(date, "%x");
}

void get_current_datetime(std::string& datetime)
{
   get_current_daytime_generic(datetime, "%c");
}


} // namespace orion
