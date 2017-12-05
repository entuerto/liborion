//
// TestStdOutput.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
// 
#include <orion/unittest/TestStdOutput.h>

#include <orion/StringUtils.h>
#include <orion/unittest/Test.h>
#include <orion/unittest/TestSuite.h>

#include <fmt/format.h>

#include <iomanip>

using namespace std::chrono_literals;

namespace orion
{
namespace unittest
{

//---------------------------------------------------------------------------------------

StdOutput::StdOutput(std::ostream& stream, ReportLevel report_level /* = Error */)
   : _stream(stream)
   , _report_level(report_level)
   , _indent(3)
{
}

StdOutput::~StdOutput() = default;

void StdOutput::write_header(int test_count)
{
   _stream << "\n"
           << fmt::format("Running {0} test {1}...\n", test_count, plural(test_count, "case", "cases"))
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
         _stream << std::setw(_indent) << "" << fmt::format("Skipped: {}\n", item.msg);
         continue;
      }

      _stream << std::setw(_indent) << ""
              << fmt::format("Failed: file {0}, line {1}\n", item.src_loc.file_name, 
                                                             item.src_loc.line_number);

      if (not item.msg.empty())
      {
         _stream << std::setw(_indent) << "" << fmt::format("   Message:  {}\n", item.msg);
      }

      if (item.result == Result::Exception)
         continue;

      _stream << std::setw(_indent) << "" << fmt::format("   Expected: {}\n", item.expected);

      _stream << std::setw(_indent) << "" << fmt::format("   Actual:   {}\n", item.actual);
   }
}

void StdOutput::write_suite_summary(const Suite& suite)
{
   if (_report_level == ReportLevel::Error)
      return;

   _stream << "\n" << fmt::format("Test Suite {}: \n", suite.name());

   auto s = suite.stats();

   _stream << fmt::format("{0:4} {1} out of {2} passed\n", s.passed_count,
                 plural(s.passed_count, "Test", "Tests"), s.count);

   if (s.failed_item_count > 0)
      _stream << fmt::format("{0:4} {1} out of {2} failed\n", s.failed_count,
                    plural(s.failed_count, "Test", "Tests"), s.count);

   if (s.skipped_count > 0)
      _stream << fmt::format("{0:4} {1} out of {2} skipped\n", s.skipped_count,
                    plural(s.skipped_count, "Test", "Tests"), s.count);

   _stream << fmt::format("{0:4} ms", s.time_elapsed.count()) << "\n";

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
      _stream << fmt::format("Failure: {0} out of {1} tests failed.\n", 
                    stats.failed_count,
                    stats.count);
   }
   else
   {
      _stream << fmt::format("Success: {} tests passed.\n", stats.count);
   }

   _stream << "---\n";

   _stream << fmt::format("{0:4} Tests out of {1} passed\n", stats.passed_count, stats.count);

   _stream << fmt::format("{0:4} Tests out of {1} failed\n", stats.failed_count, stats.count);

   _stream << fmt::format("{0:4} Tests out of {1} skipped\n", stats.skipped_count, stats.count);

   _stream << fmt::format("{0:4} Test assertations out of {1} passed\n", 
                 stats.passed_item_count, 
                 stats.item_count);

   _stream << fmt::format("{0:4} Test assertations out of {1} failed\n",
                 stats.failed_item_count,
                 stats.item_count);

   _stream << fmt::format("{0:4} Test assertations out of {1} skipped\n",
                 stats.skipped_item_count,
                 stats.item_count);

   _stream << "---\n" << fmt::format("Test time: {} ms.\n\n", stats.time_elapsed.count());
}

void StdOutput::write_test_case(int indent, const Test& test)
{
   auto& r = test.test_result();

   if (not test.enabled())
   {
      _stream << "\n" << std::setw(indent) << "" << fmt::format("Test {}: \n", test.label());

      _stream << std::setw(indent) << ""
              << fmt::format("   Skipped: {}\n", test.disabled_reason());
      return;
   }

   std::size_t count = r.passed_item_count() + r.failed_item_count();

   _stream << "\n" << std::setw(indent) << "" << fmt::format("Test {}: \n", test.label());

   _stream << std::setw(indent) << ""
           << fmt::format("{0:4} assertations out of {1} passed\n", r.passed_item_count(), count);

   if (r.failed_item_count() > 0)
      _stream << std::setw(indent) << ""
              << fmt::format("{0:4} assertations out of {1} failed\n", 
                    r.failed_item_count(),
                    count);

   _stream << std::setw(indent) << "" << fmt::format("{:4} ms\n", r.time_elapsed().count());
}

} // namespace unittest
} // namespace orion
