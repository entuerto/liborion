// TestStdOutput.cpp
//
// Copyright 2010 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.
//

#include <iomanip>

#include <orion/unittest/Test.h>
#include <orion/unittest/TestStdOutput.h>
#include <orion/unittest/TestResultItem.h>

namespace orion
{
namespace unittest
{

TestStdOutput::TestStdOutput(std::ostream& stream)
	: TestOutput(),
   _stream(stream)
{
}

TestStdOutput::~TestStdOutput()
{
}

void TestStdOutput::write(const TestResult::SharedPtr& test_result)
{
   if (not test_result->failed())
      return;

   _stream << "Test "
           << test_result->name() << ": \n";

   const TestResultItem::SharedPtrVector& result_items = test_result->result_items();

   TestResultItem::SharedPtrVector::const_iterator iter = result_items.begin();
   TestResultItem::SharedPtrVector::const_iterator end  = result_items.end();

   for ( ; iter != end; ++iter)
   {
      if ((*iter)->result() == TestResultItem::Passed)
         continue;

      _stream << "   "
              << (((*iter)->result() == TestResultItem::Passed) ? "Passed: " : "Failed: ")
              << (*iter)->message()
              << ", file " << (*iter)->file_name()
              << ", line " << (*iter)->line_number()
              << "\n";
   }
}

void TestStdOutput::write_summary(int failure_count, int failed_item_count, int test_count, double time_elapsed)
{
   if (failure_count > 0)
   {
      _stream << "FAILURE: " << failure_count
              << " out of " << test_count
              << " tests failed. (" << failed_item_count << " failures)\n";
   }
   else
   {
      _stream << "Success: "
              << test_count
              << " tests passed.\n";
   }

   _stream << "Test time: "
           << std::setprecision(2) << time_elapsed
           << " seconds.\n";
}

TestOutput::SharedPtr TestStdOutput::create(std::ostream& stream)
{
   return TestOutput::SharedPtr(new TestStdOutput(stream));
}

} // namespace orion
} // namespace unittest
