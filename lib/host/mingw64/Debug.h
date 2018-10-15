//
// Debug.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_HOST_MINGW64_DEBUG_H
#define ORION_HOST_MINGW64_DEBUG_H

#include <orion/Orion-Stddefs.h>

#include <orion/debug/Stacktrace.h>

#include <system_error>
#include <vector>

namespace orion
{
namespace mingw64
{

std::error_code capture_stacktrace(std::vector<debug::Frame>& frames, int skip, int max_depth);

} // namespace mingw64
} // namespace orion

#endif // ORION_HOST_MINGW64_DEBUG_H
