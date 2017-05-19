// Timer.cpp
//
// Copyright 2010 tomas <tomasp@videotron.ca>
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
//

#include <orion/Timer.h>

namespace orion
{

Timer::Timer()
   : _time_point_start()
   , _time_point_end()
{
}

Timer::~Timer()
{
}

void Timer::start()
{
   _time_point_start = std::chrono::system_clock::now();
}

void Timer::stop()
{
   _time_point_end = std::chrono::system_clock::now();
}

void Timer::reset()
{
   start();
}

std::chrono::milliseconds Timer::elapsed() const
{
   return std::chrono::duration_cast<std::chrono::milliseconds>(_time_point_end -
                                                                _time_point_start);
}
}
