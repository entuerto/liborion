//
// TestOptions.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_OPTIONS_H
#define ORION_UNITTEST_OPTIONS_H

#include <orion/Config.h>

#include <chrono>
#include <functional>
#include <string>

namespace orion
{
namespace unittest
{
namespace option
{

struct Label
{
   std::string text;
};

struct Description
{
   std::string text;
};

struct SetupFunc
{
   std::function<void()> func;
};

struct TeardownFunc
{
   std::function<void()> func;
};

struct Enabled
{
};

struct Disabled
{
   std::string reason;
};

struct Timeout
{
   std::chrono::milliseconds value;
};

} // namespace option
} // namespace unittest
} // namespace orion

#endif /* ORION_UNITTEST_OPTIONS_H */
