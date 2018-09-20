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

#if defined(__clang__)
#define ORION_CLANG 
#endif

#if defined(__GNUC__)
#define ORION_GCC   
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103L)
#define ORION_CXX11 
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201402L)
#define ORION_CXX14 
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201703L)
#define ORION_CXX17 
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
