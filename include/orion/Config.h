//
// Config.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_CONFIG_H
#define ORION_CONFIG_H

#if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) 
// linux:
#include <orion/config/Linux.h>

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
// BSD:
#include <orion/config/Bsd.h>

#elif defined(__CYGWIN__)
// cygwin is not win32:
#include <orion/config/Cygwin.h>

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
// win32:
#include <orion/config/Windows.h>

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// MacOS
#include <orion/config/Macos.h>

#else
#endif

#endif // ORION_CONFIG_H
