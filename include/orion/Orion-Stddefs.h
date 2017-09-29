// Orion-Stddefs.h
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

#ifndef ORION_STDDEFS_H
#define ORION_STDDEFS_H

#ifdef _MSC_VER
   #ifdef ORION_SHARED_EXPORTS
      #define API_EXPORT __declspec(dllexport)
   #else
      #define API_EXPORT __declspec(dllimport)
   #endif
#else
   #define API_EXPORT
#endif

#ifndef NO_COPY
#define NO_COPY(klass) \
   klass(const klass&) = delete; \
   klass& operator=(const klass&) = delete;
#endif

#ifndef NO_MOVE
#define NO_MOVE(klass) \
   klass(klass&&) = delete; \
   klass& operator=(klass&&) = delete;   
#endif
   
#endif
