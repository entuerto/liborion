//
// Macros.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_MACROS_H
#define ORION_MACROS_H

#include <orion/Config.h>

namespace orion
{

// Stringify an identifier
#define ORION_TOSTRING_IMPL(...) #__VA_ARGS__
#define ORION_TOSTRING(...) ORION_TOSTRING_IMPL(__VA_ARGS__)

// Macros for string concatenation
#define ORION_CAT_IMPL(s1, s2) s1##s2
#define ORION_CAT(s1, s2) ORION_CAT_IMPL(s1, s2)

// Macros for anonymous variable name generation
#ifdef __COUNTER__ // not standard and may be missing for some compilers
#   define ORION_ANONYMOUS(x) ORION_CAT(x, __COUNTER__)
#else // __COUNTER__
#   define ORION_ANONYMOUS(x) ORION_CAT(x, __LINE__)
#endif // __COUNTER__

#if defined(ORION_CLANG) || defined(ORION_GCC)
#   define ORION_LIKELY(x) __builtin_expect(!!(x), 1)
#   define ORION_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#   define ORION_LIKELY(x) (!!(x))
#   define ORION_UNLIKELY(x) (!!(x))
#endif

//
// ORION_ASSUME(cond)
//
// Tell the optimizer that the predicate cond must hold. It is unspecified
// whether or not cond is actually evaluated.
//
#ifdef ORION_MSVC
#   define ORION_ASSUME(cond) __assume(cond)
#elif defined(ORION_GCC)
#   define ORION_ASSUME(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
#else
#   define ORION_ASSUME(cond) static_cast<void>((cond) ? 0 : 0)
#endif

} // namespace orion

#endif // ORION_MACROS_H