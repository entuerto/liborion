//
// ErrorMacros.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
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
