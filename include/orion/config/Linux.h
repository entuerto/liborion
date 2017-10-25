//
// Linux.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_CONFIG_LINUX_H
#define ORION_CONFIG_LINUX_H

#define ORION_LINUX 1

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

//
// If glibc is past version 2 then we definitely have
// gettimeofday, earlier versions may or may not have it:
//
#if defined(__GLIBC__) && (__GLIBC__ >= 2)
#define ORION_HAS_GETTIMEOFDAY
#endif

#include <orion/config/Posix.h>

#endif // ORION_CONFIG_LINUX_H
