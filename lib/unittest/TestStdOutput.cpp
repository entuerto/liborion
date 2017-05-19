// TestStdOutput.cpp
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#include <orion/unittest/TestStdOutput.h>

#include <orion/StringUtils.h>
#include <orion/unittest/Test.h>
#include <orion/unittest/TestSuite.h>

#include <boost/format.hpp>

#include <iomanip>

using namespace std::chrono_literals;

namespace orion
{
namespace unittest
{

//---------------------------------------------------------------------------------------

StdOutput::StdOutput(std::ostream& stream, ReportLevel report_level /* = Error */)
   : Output()
   , _stream(stream)
   , _report_level(report_level)
   , _indent(3)
{
}

StdOutput::~StdOutput()
{
}

void StdOutput::write_header(int test_count)
{
   _stream << "\n"
           << boost::format("Running %d test %s...\n") % test_count %
                 plural(test_count, "case", "cases")
           << "\n";
}

void StdOutput::write_suite_header(const Suite& /* suite */)
{
}

void StdOutput::write(const TestResult& test_result)
{
   if (not test_result.failed())
      return;

   auto& items = test_result.result_items();

   for (auto&& item : items)
   {
      if (item.result == Result::Passed)
         continue;

      if (item.result == Result::Skipped)
      {
         _stream << std::setw(_indent) << "" << boost::format("Skipped: %s\n") % item.msg;
         continue;
      }

      _stream << std::setw(_indent) << ""
              << boost::format("Failed: file %s, line %s\n") % item.src_loc.file_name %
                    item.src_loc.line_number;

      if (not item.msg.empty())
      {
         _stream << std::setw(_indent) << "" << boost::format("   Message:  %s\n") % item.msg;
      }

      if (item.result == Result::Exception)
         continue;

      _stream << std::setw(_indent) << "" << boost::format("   Expected: %s\n") % item.expected;

      _stream << std::setw(_indent) << "" << boost::format("   Actual:   %s\n") % item.actual;
   }
}

void StdOutput::write_suite_summary(const Suite& suite)
{
   if (_report_level == ReportLevel::Error)
      return;

   _stream << "\n" << boost::format("Test Suite %s: \n") % suite.name();

   auto s = suite.stats();

   _stream << boost::format("%4d %s out of %d passed\n") % s.passed_count %
                 plural(s.passed_count, "Test", "Tests") % s.count;

   if (s.failed_item_count > 0)
      _stream << boost::format("%4d %s out of %d failed\n") % s.failed_count %
                    plural(s.failed_count, "Test", "Tests") % s.count;

   if (s.skipped_count > 0)
      _stream << boost::format("%4d %s out of %d skipped\n") % s.skipped_count %
                    plural(s.skipped_count, "Test", "Tests") % s.count;

   _stream << boost::format("%4d ms") % s.time_elapsed.count() << "\n";

   if (_report_level != ReportLevel::Detailed)
      return;

   for (const auto& test : suite.test_cases())
   {
      write_test_case(3, test);
   }
}

void StdOutput::write_summary(const OutputStats& stats)
{
   _stream << "\n";

   if (stats.failed_item_count > 0)
   {
      _stream << boost::format("Failure: %d out of %d tests failed.\n") % stats.failed_count %
                    stats.count;
   }
   else
   {
      _stream << boost::format("Success: %d tests passed.\n") % stats.count;
   }

   _stream << "---\n";

   _stream << boost::format("%4d Tests out of %d passed\n") % stats.passed_count % stats.count;

   _stream << boost::format("%4d Tests out of %d failed\n") % stats.failed_count % stats.count;

   _stream << boost::format("%4d Tests out of %d skipped\n") % stats.skipped_count % stats.count;

   _stream << boost::format("%4d Test assertations out of %d passed\n") % stats.passed_item_count %
                 stats.item_count;

   _stream << boost::format("%4d Test assertations out of %d failed\n") % stats.failed_item_count %
                 stats.item_count;

   _stream << boost::format("%4d Test assertations out of %d skipped\n") %
                 stats.skipped_item_count % stats.item_count;

   _stream << "---\n" << boost::format("Test time: %d ms.\n\n") % stats.time_elapsed.count();
}

void StdOutput::write_test_case(int indent, const Test& test)
{
   auto& r = test.test_result();

   if (not test.enabled())
   {
      _stream << "\n" << std::setw(indent) << "" << boost::format("Test %s: \n") % test.label();

      _stream << std::setw(indent) << ""
              << boost::format("   Skipped: %s\n") % test.disabled_reason();
      return;
   }

   int count = r.passed_item_count() + r.failed_item_count();

   _stream << "\n" << std::setw(indent) << "" << boost::format("Test %s: \n") % test.label();

   _stream << std::setw(indent) << ""
           << boost::format("%4d assertations out of %d passed\n") % r.passed_item_count() % count;

   if (r.failed_item_count() > 0)
      _stream << std::setw(indent) << ""
              << boost::format("%4d assertations out of %d failed\n") % r.failed_item_count() %
                    count;

   _stream << std::setw(indent) << "" << boost::format("%4d ms") % r.time_elapsed().count() << "\n";
}

} // namespace orion
} // namespace unittest
