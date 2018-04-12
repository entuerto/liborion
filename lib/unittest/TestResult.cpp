//
// TestResult.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/unittest/Test.h>
#include <orion/unittest/TestResult.h>

namespace orion
{
namespace unittest
{

TestResult::TestResult()
   : _counters()
   , _assertions()
{
}

bool TestResult::passed() const
{
   return not failed();
}

bool TestResult::failed() const
{
   return _counters.failed > 0;
}

bool TestResult::skipped() const
{
   return _counters.total() == _counters.skipped;
}

const Counters& TestResult::counters() const
{
   return _counters;
}

std::chrono::nanoseconds TestResult::time_elapsed() const
{
   return _time_point_end - _time_point_start;
}

const std::vector<Assertion>& TestResult::assertions() const
{
   return _assertions;
}

void TestResult::on_start()
{
   _time_point_start = std::chrono::high_resolution_clock::now();
}

void TestResult::on_end()
{
   _time_point_end = std::chrono::high_resolution_clock::now();
}

} // namespace unittest
} // namespace orion
