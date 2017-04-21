// TestOptions.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#ifndef ORION_UNITTEST_OPTIONS_H
#define ORION_UNITTEST_OPTIONS_H

#include <chrono>
#include <functional>
#include <string>

#include <orion/Orion-Stddefs.h>

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
   std::function<void ()> func;
};

struct TeardownFunc 
{
   std::function<void ()> func;
};

struct Enabled {};

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
