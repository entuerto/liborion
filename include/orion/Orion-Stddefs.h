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
   klass(klass&&) noexcept = delete; \
   klass& operator=(klass&&) noexcept = delete;   
#endif

#ifndef DEFAULT_COPY
#define DEFAULT_COPY(klass) \
   klass(const klass&) = default; \
   klass& operator=(const klass&) = default;
#endif

#ifndef DEFAULT_MOVE
#define DEFAULT_MOVE(klass) \
   klass(klass&&) noexcept = default; \
   klass& operator=(klass&&) noexcept = default;   
#endif
   
#endif
