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

#include <iomanip>

#include <orion/unittest/Test.h>
#include <orion/unittest/TestResultItem.h>

#include <boost/format.hpp>

namespace orion
{
namespace unittest
{
//---------------------------------------------------------------------------------------

static std::string pluralize(int n, const std::string& s, const std::string& p)
{
   return (n > 1) ? p : s;
}

//---------------------------------------------------------------------------------------

TestStdOutput::TestStdOutput(std::ostream& stream, int report_level) :
	TestOutput(),
   _stream(stream),
   _report_level(report_level)
{
}

TestStdOutput::~TestStdOutput()
{
}

void TestStdOutput::write_header(const std::string& suite_name, int test_count) const
{
   _stream << boost::format("Running %s test %s...") 
                 % test_count 
                 % pluralize(test_count, "case", "cases")
           << "\n";
}

void TestStdOutput::write(const TestResult* test_result) const
{
   if (_report_level == 0 and not test_result->failed())
      return;

   _stream << boost::format("\nTest %s: \n") % test_result->name();

   auto& items = test_result->result_items();

   if (_report_level == 1)
   {
      _stream << boost::format("   %d test case out of %d passed\n")
                    % test_result->passed_item_count()
                    % items.size();
              
      if (test_result->failed_item_count() > 0)
         _stream << boost::format("   %d test case out of %d failed\n")
                       % test_result->failed_item_count()
                       % items.size();

      _stream << boost::format("   %d ms\n") % test_result->time_elapsed().count();

      if (test_result->failed_item_count() > 0)
         _stream << "\n";
   }
   
   for (auto&& item : items)
   {
      if (item->result() == Result::Passed)
         continue;

      _stream << boost::format("   Failed: %s, file %s, line %s\n")
                    % item->message()
                    % item->file_name()
                    % item->line_number();
   }
}

void TestStdOutput::write_summary(int failure_count, int failed_item_count, int test_count, std::chrono::milliseconds time_elapsed) const
{
   _stream << "\n";

   if (failure_count > 0)
   {
      _stream << boost::format("Failure: %d out of %d tests failed. (%d %s)\n") 
                    % failure_count
                    % test_count
                    % failed_item_count 
                    % pluralize(failed_item_count, "failure", "failures");
   }
   else
   {
      _stream << boost::format("Success: %d tests passed.\n") % test_count;
   }

   _stream << "---\n"
           << boost::format("Test time: %d ms.\n") % time_elapsed.count();
}

std::unique_ptr<TestOutput> TestStdOutput::create(std::ostream& stream, int report_level /* = 0 */)
{
   return std::make_unique<TestStdOutput>(stream, report_level);
}

} // namespace orion
} // namespace unittest
