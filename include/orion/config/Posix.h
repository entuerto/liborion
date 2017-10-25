//
// Posix.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_CONFIG_POSIX_H
#define ORION_CONFIG_POSIX_H

#if defined(ORION_HAS_UNISTD_H)

#include <unistd.h>

// POSIX version 6 requires <stdint.h>
#if defined(_POSIX_VERSION) && (_POSIX_VERSION >= 200100)
# define ORION_HAS_STDINT_H
#endif

// POSIX version 2 requires <dirent.h>
#if defined(_POSIX_VERSION) && (_POSIX_VERSION >= 199009L)
# define ORION_HAS_DIRENT_H
#endif

// POSIX version 3 requires <signal.h> to have sigaction:
#if defined(_POSIX_VERSION) && (_POSIX_VERSION >= 199506L)
# define ORION_HAS_SIGACTION
#endif

// POSIX defines _POSIX_THREADS > 0 for pthread support,
// however some platforms define _POSIX_THREADS without
// a value, hence the (_POSIX_THREADS+0 >= 0) check.
#if defined(_POSIX_THREADS) && (_POSIX_THREADS+0 >= 0) && !defined(ORION_HAS_WINTHREADS) && !defined(ORION_HAS_MPTASKS)
# define ORION_HAS_PTHREADS
#endif

// ORION_HAS_NANOSLEEP:
// This is predicated on _POSIX_TIMERS:
#if (defined(_POSIX_TIMERS) && (_POSIX_TIMERS+0 >= 0)) 
# define ORION_HAS_NANOSLEEP
#endif

#endif

#endif // ORION_CONFIG_POSIX_H
