// NotImplementedException.cpp
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

#include <orion/NotImplementedException.h>

namespace orion
{
// ---------------------------------------------------------------------------
// ---- NotImplementedException ----

NotImplementedException::NotImplementedException(const std::string& text, const SourceLocation& src_loc) :
   Exception(text, src_loc)
{
}

NotImplementedException::NotImplementedException(const NotImplementedException& other) :
   Exception(other)
{
}

NotImplementedException::NotImplementedException(NotImplementedException&& other) :
   Exception(std::move(other))
{
}

NotImplementedException& NotImplementedException::operator=(const NotImplementedException& other)
{
   if (this == &other) {
      return *this;
   }

   Exception::operator=(other);

   return *this ;
}

NotImplementedException& NotImplementedException::operator=(NotImplementedException&& other)
{
   Exception::operator=(std::move(other));

   return *this ;
}

} // namespace orion
