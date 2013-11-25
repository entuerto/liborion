// ErrorMacros.h
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

#ifndef ORION_ERRORMACROS_H
#define ORION_ERRORMACROS_H

#ifndef RETURN_IF_FAIL
#define RETURN_IF_FAIL(expr) \
{                                         \
   if (expr) { } else                     \
   {	                                  \
      LOG(Error) << "Condition failed ( " \
                 << #expr                 \
                 << " )";                 \
   return;				  \
   }				          \
}
#endif

#ifndef RETURN_VALUE_IF_FAIL
#define RETURN_VALUE_IF_FAIL(expr, val)   \
{                                         \
   if (expr) { } else                     \
   {	                                  \
      LOG(Error) << "Condition failed ( " \
                 << #expr                 \
                 << " )";                 \
   return (val);			  \
   }				          \
}
#endif

#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(except, message) \
   throw except((message), __FILE__, __LINE__, __FUNCTION__);
#endif

#ifndef THROW_STDEXCEPTION
#define THROW_STDEXCEPTION(except, message) \
   throw except((message));
#endif

#ifndef THROW_IF
#define THROW_IF(condition, except) \
   if ((condition)) { \
      throw except(#except ": Condition " #condition, __FILE__, __LINE__, __FUNCTION__); \
   }
#endif

#ifndef THROW_IF3
#define THROW_IF3(condition, except, message) \
   if ((condition)) { \
      throw except((message), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
   }
#endif

#endif

