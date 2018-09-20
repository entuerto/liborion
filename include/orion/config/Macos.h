//
// Macos.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_CONFIG_MACOS_H
#define ORION_CONFIG_MACOS_H

#if defined(__MACH__)
#define ORION_MACOS 
#endif

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
#define ORION_HAS_GETTIMEOFDAY
#define ORION_HAS_PTHREADS
#define ORION_HAS_SIGACTION

#include <orion/config/Posix.h>

#endif // ORION_CONFIG_MACOS_H
