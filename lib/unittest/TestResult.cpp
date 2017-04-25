// TestResult.cpp
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// 
#include <orion/unittest/TestResult.h>
#include <orion/unittest/Test.h>

namespace orion
{
namespace unittest
{

TestResult::TestResult(const std::string& name, const std::string& suite_name) :
   _name(name),
   _suite_name(suite_name),
   _failed_item_count(0),
   _passed_item_count(0),
   _skipped_item_count(0),
   _time_point_start(),
   _time_point_end(),
   _result_items()
{
}

/*!
 */
std::string TestResult::name() const
{
   return _name;
}

/*!
 */
std::string TestResult::suite_name() const
{
   return _suite_name;
}

/*!
 */
bool TestResult::passed() const
{
   return not failed();
}

/*!
 */
bool TestResult::failed() const
{
   return _failed_item_count > 0;
}

size_t TestResult::item_count() const
{
   return _result_items.size();
}

/*!
 */
std::size_t TestResult::failed_item_count() const
{
   return _failed_item_count;
}

/*!
 */
std::size_t TestResult::passed_item_count() const
{
   return _passed_item_count;
}

/*!
 */
std::size_t TestResult::skipped_item_count() const
{
   return _skipped_item_count;
}

/*!
 */
std::chrono::milliseconds TestResult::time_elapsed() const
{
   return std::chrono::duration_cast<std::chrono::milliseconds>(_time_point_end - _time_point_start);
}

/*!
 */
const std::vector<ResultItem>& TestResult::result_items() const
{
   return _result_items;
}

/*!
 */
void TestResult::on_start()
{
   _time_point_start = std::chrono::steady_clock::now();
}

/*!
 */
void TestResult::on_end()
{
   _time_point_end = std::chrono::steady_clock::now();
}

} // namespace orion
} // namespace unittest
