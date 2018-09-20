//
// Bsd.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_CONFIG_BSD_H
#define ORION_CONFIG_BSD_H

#define ORION_BSD 1

#define ORION_FREEBSD   __FreeBSD__
#define ORION_NETBSD    __OpenBSD__
#define ORION_OPENBSD   __OpenBSD__
#define ORION_DRAGONFLY __DragonFly__

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

#define ORION_HAS_UNISTD_H
#define ORION_HAS_SCHED_YIELD
#define ORION_HAS_NANOSLEEP
#define ORION_HAS_GETTIMEOFDAY
#define ORION_HAS_PTHREADS
#define ORION_HAS_PTHREAD_MUTEXATTR_SETTYPE
#define ORION_HAS_SIGACTION

#include <orion/config/Posix.h>

#endif // ORION_CONFIG_BSD_H
