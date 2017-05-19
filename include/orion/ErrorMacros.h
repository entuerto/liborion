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
#define RETURN_IF_FAIL(expr)                                   \
   \
{                                                        \
      if (expr)                                                \
      {                                                        \
      }                                                        \
      else                                                     \
      {                                                        \
         LOG(Error) << "Condition failed ( " << #expr << " )"; \
         return;                                               \
      }                                                        \
   \
}
#endif

#ifndef RETURN_VALUE_IF_FAIL
#define RETURN_VALUE_IF_FAIL(expr, val)                        \
   \
{                                                        \
      if (expr)                                                \
      {                                                        \
      }                                                        \
      else                                                     \
      {                                                        \
         LOG(Error) << "Condition failed ( " << #expr << " )"; \
         return (val);                                         \
      }                                                        \
   \
}
#endif

// throw_error

#endif
