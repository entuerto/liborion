//
// Debug.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_HOST_WIN32_DEBUG_H
#define ORION_HOST_WIN32_DEBUG_H

#include <orion/Orion-Stddefs.h>

#include <orion/debug/Stacktrace.h>

#include <system_error>
#include <vector>

namespace orion
{
namespace win32
{

std::error_code capture_stacktrace(std::vector<debug::Frame>& frames, int skip, int max_depth);

} // namespace win32
} // namespace orion

#endif // ORION_HOST_WIN32_DEBUG_H
