//
// TestStdOutput.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
// 
#include <orion/unittest/TestStdOutput.h>

#include <orion/Iterator.h>
#include <orion/String.h>
#include <orion/unittest/Test.h>
#include <orion/unittest/TestSuite.h>

#include <fmt/format.h>

#include <chrono>
#include <iomanip>

namespace orion
{
namespace unittest
{

using namespace std::chrono_literals;
using namespace std::string_literals;

using dbl_milliseconds = std::chrono::duration<double, std::milli>;

static const std::string header_short = 
   "Test Suites                                Passed Failed Skipped Total  Elapsed\n";

static const std::string text_suite =
   "  {0:<40} {1:^6} {2:^6} {3:^6}  {4:^6}  {5:.2f} ms\n";

static const std::string text_item =
   "    {0:<38} {1:^6} {2:^6} {3:^6}  {4:^6}  {5} ms\n";

static const std::string text_item_failed = R"(
  Failed: file {0}, line {1}
     Expected: {2}
     Actual:   {3}
)";

static const std::string text_item_failed_msg = R"(
  Failed: file {0}, line {1}
     Message:  {2}
     Expected: {3}
     Actual:   {4}
)";

static const std::string text_item_skipped = R"(
  Skipped: {}
     Message: {}
)";

static const std::string text_item_exception = R"(
  Exception: file {0}, line {1}
     Message: {2}
)";

static const std::string text_summary = R"(
--------------------------------------------------------------------------------
Tests:        {count:6}  Passed: {passed:6}  Failed: {failed:6}  Skipped: {skipped:6}
Assertations: {item_count:6}  Passed: {item_passed:6}  Failed: {item_failed:6}  Skipped: {item_skipped:6}
--------------------------------------------------------------------------------
Elapsed time: {time_elapsed:.2f} ms
)";

//---------------------------------------------------------------------------------------

StdOutput::StdOutput(std::ostream& stream, ReportLevel report_level /* = Error */)
   : _stream(stream)
   , _report_level(report_level)
{
}

StdOutput::~StdOutput() = default;

void StdOutput::write_header(uint64_t test_count)
{
   _stream << "\n"
           << fmt::format("Running {0} test {1}...\n", test_count, plural(test_count, "case"s, "cases"s));
}

void StdOutput::suite_start(const TestSuite& /* suite */)
{
}

void StdOutput::write(const TestResult& test_result)
{
   for (const auto& item : test_result.assertions())
   {
      if (item.result == Result::Passed)
         continue;

      if (item.result == Result::Skipped)
      {
         _stream << fmt::format(text_item_skipped, test_result.name(), item.msg);
         continue;
      }

      if (item.result == Result::Exception)
      {
         _stream << fmt::format(text_item_exception, item.src_loc.file_name,
                                                     item.src_loc.line_number,
                                                     item.msg);
         continue;
      }

      if (item.msg.empty())
      {
         _stream << fmt::format(text_item_failed, item.src_loc.file_name,
                                                  item.src_loc.line_number,
                                                  item.expected,
                                                  item.actual);
      }
      else
      {
         _stream << fmt::format(text_item_failed_msg, item.src_loc.file_name,
                                                      item.src_loc.line_number,
                                                      item.msg,
                                                      item.expected,
                                                      item.actual);
      }

      if (_report_level == ReportLevel::Detailed)
      {
         auto& result_counters = test_result.counters();

         _test_stats.emplace(test_result.suite_name(), TestStats{
            test_result.name(),
            result_counters,
            test_result.time_elapsed()});
      }
   }
}

void StdOutput::suite_end(const TestSuite& suite)
{
   if (_report_level == ReportLevel::Error)
      return;

   _test_suite_stats.emplace(suite.name(), suite.stats());
}

void StdOutput::write_footer(const Totals& totals)
{
   if (_report_level != ReportLevel::Error)
      write_sections();

   _stream << "\n" 
           << (totals.assertions.failed > 0 ? "Failure" : "Success");

   auto millis = std::chrono::duration_cast<dbl_milliseconds>(totals.time_elapsed);

   _stream << fmt::format(text_summary, fmt::arg("count", totals.tests.total()),
                                        fmt::arg("passed", totals.tests.passed),
                                        fmt::arg("failed", totals.tests.failed),
                                        fmt::arg("skipped", totals.tests.skipped),
                                        fmt::arg("item_count", totals.assertions.total()),
                                        fmt::arg("item_passed", totals.assertions.passed),
                                        fmt::arg("item_failed", totals.assertions.failed),
                                        fmt::arg("item_skipped", totals.assertions.skipped),
                                        fmt::arg("time_elapsed", millis.count()));
}

void StdOutput::write_sections()
{
   // Header
   _stream << "\n" 
           << header_short;

   TestSuiteStats total{};

   // Test suite results
   for (const auto& item : _test_suite_stats)
   {
      auto& s = item.second;

      total += s;

      auto millis = std::chrono::duration_cast<dbl_milliseconds>(s.time_elapsed);

      _stream << fmt::format(text_suite, s.label, s.tests.passed, 
                                                  s.tests.failed, 
                                                  s.tests.skipped, 
                                                  s.tests.total(), 
                                                  millis.count());
      
      if (_report_level == ReportLevel::Detailed)
      {
         auto range = make_range(_test_stats.equal_range(item.first));

         for (const auto& test_item : range)
         {
            auto& i = test_item.second;
            auto& counters = i.assertions;

            auto ms = std::chrono::duration_cast<dbl_milliseconds>(i.time_elapsed);

            _stream << fmt::format(text_item, fit_text(i.label, 38), counters.passed, 
                                                                     counters.failed, 
                                                                     counters.skipped,
                                                                     counters.total(),
                                                                     ms.count());
         }
         _stream << "\n";
      }
   }

   // Totals
   auto millis = std::chrono::duration_cast<dbl_milliseconds>(total.time_elapsed);

   std::string label = (_report_level == ReportLevel::Detailed) ? "Total" : " ";

   _stream << fmt::format(text_suite, label, total.tests.passed, 
                                             total.tests.failed, 
                                             total.tests.skipped, 
                                             total.tests.total(), 
                                             millis.count());
}

std::string StdOutput::fit_text(const std::string& text, int len)
{
   return (text.size() > len) ? text.substr(0, len - 3) + "..." : text;
}

} // namespace unittest
} // namespace orion
