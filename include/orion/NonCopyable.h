// NotCopyable.h
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

#ifndef ORION_NONCOPYABLE_H
#define ORION_NONCOPYABLE_H

namespace orion
{
//! Makes a sub class not copyable
/*!
    This class is to be used when a sub class should
    not be copyable.
 */
class NonCopyable
{
protected:
   NonCopyable() {}
   ~NonCopyable() {}

private:
   NonCopyable(const NonCopyable&);
   NonCopyable& operator=(const NonCopyable&);
};

} // namespace orion
#endif /* ORION_NONCOPYABLE_H */
