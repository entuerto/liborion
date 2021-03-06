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
#include <orion/unittest/TestRunner.h>
#include <orion/unittest/TestSuite.h>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <chrono>
#include <iomanip>

namespace orion
{
namespace unittest
{

using namespace std::chrono_literals;
using namespace std::string_literals;

using dbl_milliseconds = std::chrono::duration<double, std::milli>;

// clang-format off

static const std::string header_short = 
   "Test Suites                                Passed Failed Skipped Total  Elapsed\n";

static const std::string text_suite =
   "  {0:<40} {1:^6} {2:^6} {3:^6}  {4:^6}  {5:.2f} ms\n";

static const std::string text_item =
   "    {0:<38} {1:^6} {2:^6} {3:^6}  {4:^6}  {5} ms\n";

static const std::string text_item_failed = R"(
  Failed: file {0}, line {1}
     Value of: {2}
     Actual:   {3}
     Expected: {4}
)";

static const std::string text_item_failed_msg = R"(
  Failed: file {0}, line {1}
     Message:  {2}
     Value of: {3}
     Actual:   {4}
     Expected: {5}
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

// clang-format on

//--------------------------------------------------------------------------------------------------

StdOutput::StdOutput(std::ostream& stream, ReportLevel report_level /* = Error */)
   : _stream(stream)
   , _report_level(report_level)
{
}

StdOutput::~StdOutput() = default;

void StdOutput::runner_start(const TestRunner& runner)
{
   uint32_t test_count = runner.test_case_count();

   _stream << "\n"
           << fmt::format(
                 "Running {0} test {1}...\n", test_count, plural(test_count, "case"s, "cases"s));
}

void StdOutput::suite_start(const TestSuite& suite)
{
   _current_test_suite = suite.name();
}

void StdOutput::test_start(const Test& test)
{
   _current_test = as_string(test.label());
}

void StdOutput::test_end(const TestResult& test_result)
{
   for (const auto& item : test_result.assertions())
   {
      if (item.result() == Result::Passed)
         continue;

      const auto sl = item.source_location();

      if (item.result() == Result::Skipped)
      {
         _stream << fmt::format(text_item_skipped, _current_test, item.message());
         continue;
      }

      if (item.result() == Result::Exception)
      {
         _stream << fmt::format(text_item_exception, sl.file_name(), sl.line_number(), item.message());
         continue;
      }

      if (item.message().empty())
      {
         _stream << fmt::format(text_item_failed,
                                sl.file_name(),
                                sl.line_number(),
                                item.expression(),
                                item.actual(),
                                item.expected());
      }
      else
      {
         _stream << fmt::format(text_item_failed_msg,
                                sl.file_name(),
                                sl.line_number(),
                                item.message(),
                                item.expression(),
                                item.actual(),
                                item.expected());
      }
   }

   if (_report_level == ReportLevel::Detailed)
   {
      auto& result_counters = test_result.counters();

      _test_stats.emplace(_current_test_suite,
                          TestStats{_current_test, result_counters, test_result.elapsed_time()});
   }
}

void StdOutput::suite_end(const TestSuiteStats& stats)
{
   if (_report_level == ReportLevel::Error)
      return;

   _test_suite_stats.emplace(_current_test_suite, stats);
}

void StdOutput::runner_end(const Totals& totals)
{
   if (_report_level != ReportLevel::Error)
      write_sections();

   _stream << "\n" << (totals.assertions.failed > 0 ? "Failure" : "Success");

   auto millis = std::chrono::duration_cast<dbl_milliseconds>(totals.time_elapsed);

   _stream << fmt::format(text_summary,
                          fmt::arg("count", totals.tests.total()),
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
   _stream << "\n" << header_short;

   TestSuiteStats total{};

   // Test suite results
   for (const auto& item : _test_suite_stats)
   {
      auto& s = item.second;

      total += s;

      auto millis = std::chrono::duration_cast<dbl_milliseconds>(s.time_elapsed);

      _stream << fmt::format(text_suite,
                             s.label,
                             s.tests.passed,
                             s.tests.failed,
                             s.tests.skipped,
                             s.tests.total(),
                             millis.count());

      if (_report_level == ReportLevel::Detailed)
      {
         auto range = make_range(_test_stats.equal_range(item.first));

         for (const auto& test_item : range)
         {
            auto& i        = test_item.second;
            auto& counters = i.assertions;

            auto ms = std::chrono::duration_cast<dbl_milliseconds>(i.time_elapsed);

            _stream << fmt::format(text_item,
                                   fit_text(i.label, 38),
                                   counters.passed,
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

   _stream << fmt::format(text_suite,
                          label,
                          total.tests.passed,
                          total.tests.failed,
                          total.tests.skipped,
                          total.tests.total(),
                          millis.count());
}

std::string StdOutput::fit_text(const std::string& text, std::size_t len)
{
   return (text.size() > len) ? text.substr(0, len - 3) + "..." : text;
}

} // namespace unittest
} // namespace orion
