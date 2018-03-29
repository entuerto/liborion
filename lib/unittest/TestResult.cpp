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

TestResult::TestResult(const std::string& name, const std::string& suite_name)
   : _name(name)
   , _suite_name(suite_name)
   , _failed_item_count(0)
   , _passed_item_count(0)
   , _skipped_item_count(0)
{
}

std::string TestResult::name() const
{
   return _name;
}

std::string TestResult::suite_name() const
{
   return _suite_name;
}

bool TestResult::passed() const
{
   return not failed();
}

bool TestResult::failed() const
{
   return _failed_item_count > 0;
}

uint64_t TestResult::item_count() const
{
   return _result_items.size();
}

uint64_t TestResult::failed_item_count() const
{
   return _failed_item_count;
}

uint64_t TestResult::passed_item_count() const
{
   return _passed_item_count;
}

uint64_t TestResult::skipped_item_count() const
{
   return _skipped_item_count;
}

std::chrono::nanoseconds TestResult::time_elapsed() const
{
   return _time_point_end - _time_point_start;
}

const std::vector<ResultItem>& TestResult::result_items() const
{
   return _result_items;
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
