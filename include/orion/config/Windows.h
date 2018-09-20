//
// Windows.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_CONFIG_WINDOWS_H
#define ORION_CONFIG_WINDOWS_H

#define ORION_WINDOWS 1

#if defined(__MINGW32__)
#include <_mingw.h>
#endif

#if defined(__MINGW32__)
#define ORION_MINGW32 
#endif

#if defined(__MINGW64__)
#define ORION_MINGW64 
#endif

#if defined(_MSC_VER)
#define ORION_MSVC  
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
#ifdef ORION_SHARED_EXPORTS
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif

#if defined(__MINGW32__) && ((__MINGW32_MAJOR_VERSION > 2) || ((__MINGW32_MAJOR_VERSION == 2) && (__MINGW32_MINOR_VERSION >= 0)))
#define ORION_HAS_STDINT_H
#define ORION_HAS_DIRENT_H
#define ORION_HAS_UNISTD_H
#endif

#if defined(__MINGW32__) && (__GNUC__ >= 4)
#define ORION_HAS_GETTIMEOFDAY
#endif

#include <orion/config/Posix.h>

#endif // ORION_CONFIG_WINDOWS_H
