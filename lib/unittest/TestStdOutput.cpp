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

using namespace std::chrono_literals;

namespace orion
{
namespace unittest
{
//---------------------------------------------------------------------------------------

static std::string plural(int n, const std::string& s, const std::string& p)
{
   return (n > 1) ? p : s;
}

//---------------------------------------------------------------------------------------

TestStdOutput::TestStdOutput(std::ostream& stream, ReportLevel report_level) :
	TestOutput(),
   _stream(stream),
   _report_level(report_level),
   _results_by_testsuite(),
   _results()
{
}

TestStdOutput::~TestStdOutput()
{
}

void TestStdOutput::write_header(const std::string& suite_name, int test_count) 
{
   _stream << "\n"
           << boost::format("Running %s test %s...\n") 
                 % test_count 
                 % plural(test_count, "case", "cases")
           << "\n";
}

void TestStdOutput::write(const TestResult* test_result) 
{
   accumulate_stats(test_result);

   if (not test_result->failed())
      return;

   auto& items = test_result->result_items();
   
   for (auto&& item : items)
   {
      if (item->result() == Result::Passed)
         continue;

      _stream << boost::format("Failed: file %s, line %s\n")
                    % item->file_name()
                    % item->line_number();
      _stream << boost::format("   Expected: %s\n")
                    % item->expected();
      _stream << boost::format("   Actual:   %s\n")
                    % item->got();
   }
}

void TestStdOutput::write_summary(const TestOutputStats& stats) 
{
   ouput_short();
   ouput_detailed();

   _stream << "\n";

   if (stats.failed_item_count > 0)
   {
      _stream << boost::format("Failure: %d out of %d tests failed.\n") 
                    % stats.failed_count
                    % stats.count;
   }
   else
   {
      _stream << boost::format("Success: %d tests passed.\n") % stats.count;
   }

   _stream << "---\n";

   _stream << boost::format("%4d test out of %d passed\n")
                 % stats.passed_count
                 % stats.count;
   _stream << boost::format("%4d test out of %d failed\n")
                 % stats.failed_count
                 % stats.count;
   _stream << boost::format("%4d test cases out of %d passed\n")
                 % stats.passed_item_count
                 % stats.item_count;   
   _stream << boost::format("%4d test cases out of %d failed\n")
                 % stats.failed_item_count
                 % stats.item_count;
   _stream << "---\n"
           << boost::format("Test time: %d ms.\n\n") % stats.time_elapsed.count();
}

void TestStdOutput::accumulate_stats(const TestResult* test_result)
{
   if (test_result == nullptr)
      return;

   const auto& items = test_result->result_items();

   // By Test
   _results.insert({test_result->suite_name(), ResultStats{
      test_result->name(),
      items.size(), 
      test_result->passed_item_count(),
      test_result->failed_item_count(),
      test_result->time_elapsed()
   }});

   // By Test suite
   auto r = _results_by_testsuite.find(test_result->suite_name());
   if (r != _results_by_testsuite.end()) 
   {
      r->second.count += items.size(); 
      r->second.passed_item_count += test_result->passed_item_count();
      r->second.failed_item_count += test_result->failed_item_count();
      r->second.time_elapsed      += test_result->time_elapsed();
   }
   else
   {
      _results_by_testsuite[test_result->suite_name()] = ResultStats{
         test_result->suite_name(),
         items.size(), 
         test_result->passed_item_count(),
         test_result->failed_item_count(),
         test_result->time_elapsed()
      };
   }
}

void TestStdOutput::write_test_suite(int indent, const ResultStats& r) 
{
   _stream << "\n"
           << std::setw(indent) << ""
           << boost::format("Test Suite %s: \n") % r.name;

   _stream << std::setw(indent) << ""
           << boost::format("   %d test case out of %d passed\n")
              % r.passed_item_count
              % r.count;

   if (r.failed_item_count > 0)
      _stream << std::setw(indent) << "" 
              << boost::format("   %d test case out of %d failed\n")
                    % r.failed_item_count
                    % r.count;

   _stream << std::setw(indent) << ""
           << boost::format("   %d ms") % r.time_elapsed.count()
           << "\n";
}

void TestStdOutput::write_test_case(int indent, const ResultStats& r) 
{
   _stream << "\n"
           << std::setw(indent) << ""
           << boost::format("Test %s: \n") % r.name;

   _stream << std::setw(indent) << "" 
           << boost::format("   %d test case out of %d passed\n")
              % r.passed_item_count
              % r.count;

   if (r.failed_item_count > 0)
      _stream << std::setw(indent) << ""
              << boost::format("   %d test case out of %d failed\n")
                    % r.failed_item_count
                    % r.count;

   _stream << std::setw(indent) << ""
           << boost::format("   %d ms") % r.time_elapsed.count()
           << "\n";
}

void TestStdOutput::ouput_short() 
{
   if (_report_level != ReportLevel::Short or _results_by_testsuite.empty())
      return;

   for (const auto& result : _results_by_testsuite)
   {
      write_test_suite(0, result.second);
   }
}

void TestStdOutput::ouput_detailed() 
{
   if (_report_level != ReportLevel::Detailed or _results.empty())
      return;

   for (const auto& result : _results_by_testsuite)
   {
      write_test_suite(0, result.second);

      auto r = _results.equal_range(result.first);

      for (auto i = r.first; i != r.second; ++i)
      {
         write_test_case(3, i->second);
      }
   }
}

std::unique_ptr<TestOutput> TestStdOutput::create(std::ostream& stream, ReportLevel report_level /* = Error */)
{
   return std::make_unique<TestStdOutput>(stream, report_level);
}

} // namespace orion
} // namespace unittest
