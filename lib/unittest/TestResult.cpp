//      TestResult.cpp
//
//      Copyright 2010 tomas <tomasp@videotron.ca>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
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
   _timer(),
   _result_items()
{

}


TestResult::~TestResult()
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

/*!
 */
int TestResult::failed_item_count() const
{
   return _failed_item_count;
}

/*!
 */
int TestResult::passed_item_count() const
{
   return _passed_item_count;
}

/*!
 */
double TestResult::time_elapsed() const
{
   return _timer.elapsed();
}

/*!
 */
const TestResultItem::SharedPtrVector& TestResult::result_items() const
{
   return _result_items;
}

/*!
 */
void TestResult::on_start()
{
   _timer.start();
}

/*!
 */
void TestResult::on_failure(TestResultItem::SharedPtr item)
{
   if (item->result() != TestResultItem::Failed)
      return;

   ++_failed_item_count;
   _result_items.push_back(item);
}

/*!
 */
void TestResult::on_success(TestResultItem::SharedPtr item)
{
   if (item->result() != TestResultItem::Passed)
      return;

   ++_passed_item_count;
   _result_items.push_back(item);
}

/*!
 */
void TestResult::on_end()
{
   _timer.stop();
}

/*!
 */
TestResult::SharedPtr TestResult::create(const std::string& name, const std::string& suite_name)
{
   return SharedPtr(new TestResult(name, suite_name));
}

} // namespace orion
} // namespace unittest
