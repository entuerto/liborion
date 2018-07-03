//
// Stacktrace-Win32.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/debug/Stacktrace.h>

#include <host/mingw64/Debug.h>

namespace orion
{
namespace debug
{

Stacktrace make_stacktrace(int skip /* = 0 */, int max_depth /* = DEFAULT_MAX_DEPTH */)
{
   std::vector<Frame> frames;

   mingw64::capture_stacktrace(frames, skip, max_depth);

   return Stacktrace{std::move(frames)};
}

} // namespace debug
} // namespace orion
