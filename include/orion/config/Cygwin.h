//
// Cygwin.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_CONFIG_CYGWIN_H
#define ORION_CONFIG_CYGWIN_H

#define ORION_CYGWIN 1

#define ORION_CLANG __clang_version__

#if !defined(ORION_CLANG)
#define ORION_GCC   __GNUC__
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103L)
#define ORION_CXX11 
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201402L)
#define ORION_CXX14 
#endif

//  Default define for symbole export 
#define API_EXPORT

// See if we have POSIX threads, if we do use them, otherwise
// revert to native Win threads.
#define ORION_HAS_UNISTD_H
#include <unistd.h>

#if defined(_POSIX_THREADS) && (_POSIX_THREADS+0 >= 0) 
# define ORION_HAS_PTHREADS
# define ORION_HAS_SCHED_YIELD
# define ORION_HAS_GETTIMEOFDAY
#endif

#include <orion/config/Posix.h>

#endif // ORION_CONFIG_CYGWIN_H
