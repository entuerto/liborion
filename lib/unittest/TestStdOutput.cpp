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
#include <orion/unittest/TestStdOutput.h>

#include <orion/unittest/Test.h>
#include <orion/unittest/TestResultItem.h>

#include <iomanip>

namespace orion
{
namespace unittest
{

TestStdOutput::TestStdOutput(std::ostream& stream) :
	TestOutput(),
   _stream(stream)
{
}

TestStdOutput::~TestStdOutput()
{
}

void TestStdOutput::write(const TestResult* test_result) const
{
   if (not test_result->failed())
      return;

   _stream << "Test "
           << test_result->name() << ": \n";

   for (auto&& item : test_result->result_items())
   {
      if (item->result() == Result::Passed)
         continue;

      _stream << "   "
              << ((item->result() == Result::Passed) ? "Passed: " : "Failed: ")
              << item->message()
              << ", file " << item->file_name()
              << ", line " << item->line_number()
              << "\n";
   }
   
}

void TestStdOutput::write_summary(int failure_count, int failed_item_count, int test_count, double time_elapsed) const
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

std::unique_ptr<TestOutput> TestStdOutput::create(std::ostream& stream)
{
   return std::make_unique<TestStdOutput>(stream);
}

} // namespace orion
} // namespace unittest
