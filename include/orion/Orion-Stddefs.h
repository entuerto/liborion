//
// Orion-Stddefs.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_STDDEFS_H
#define ORION_STDDEFS_H

#include <orion/Config.h>

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
