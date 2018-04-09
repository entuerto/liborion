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
---
{passed:4} Tests out of {count} passed
{failed:4} Tests out of {count} failed
{skipped:4} Tests out of {count} skipped
{item_passed:4} Test assertations out of {item_count} passed
{item_failed:4} Test assertations out of {item_count} failed
{item_skipped:4} Test assertations out of {item_count} skipped
---
Test time: {time_elapsed:.2f} ms.
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
   auto& items = test_result.result_items();

   for (const auto& item : items)
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
   }
}

void StdOutput::suite_end(const TestSuite& suite)
{
   if (_report_level == ReportLevel::Error)
      return;

   _test_suite_stats.emplace(suite.name(), suite.stats());

   if (_report_level != ReportLevel::Detailed)
      return;

   for (const auto& test : suite.test_cases())
   {
      auto& r = test.test_result();

      _test_item_stats.emplace(suite.name(), ItemStats{
         test.label(),
         r.item_count(),
         r.passed_item_count(),
         r.failed_item_count(), 
         r.skipped_item_count(),
         r.time_elapsed()});
   }
}

void StdOutput::write_footer(const Stats& stats)
{
   if (_report_level != ReportLevel::Error)
      write_sections();

   _stream << "\n" 
           << (stats.failed_item_count > 0 ? "Failure" : "Success");

   auto millis = std::chrono::duration_cast<dbl_milliseconds>(stats.time_elapsed);

   _stream << fmt::format(text_summary, fmt::arg("count", stats.count),
                                        fmt::arg("passed", stats.passed_count),
                                        fmt::arg("failed", stats.failed_count),
                                        fmt::arg("skipped", stats.skipped_count),
                                        fmt::arg("item_count", stats.item_count),
                                        fmt::arg("item_passed", stats.passed_item_count),
                                        fmt::arg("item_failed", stats.failed_item_count),
                                        fmt::arg("item_skipped", stats.skipped_item_count),
                                        fmt::arg("time_elapsed", millis.count()));
}

void StdOutput::write_sections()
{
   // Header
   _stream << "\n" 
           << header_short;

   Stats total{};

   // Test suite results
   for (const auto& item : _test_suite_stats)
   {
      auto& s = item.second;

      total += s;

      auto millis = std::chrono::duration_cast<dbl_milliseconds>(s.time_elapsed);

      _stream << fmt::format(text_suite, s.label, s.passed_count, 
                                                  s.failed_count, 
                                                  s.skipped_count, 
                                                  s.count, 
                                                  millis.count());
      
      if (_report_level == ReportLevel::Detailed)
      {
         auto range = make_range(_test_item_stats.equal_range(item.first));

         for (const auto& test_item : range)
         {
            auto& i = test_item.second;

            auto ms = std::chrono::duration_cast<dbl_milliseconds>(i.time_elapsed);

            _stream << fmt::format(text_item, fit_text(i.label, 38), i.passed_count, 
                                                                     i.failed_count, 
                                                                     i.skipped_count,
                                                                     i.count,
                                                                     ms.count());
         }
         _stream << "\n";
      }
   }

   // Totals
   auto millis = std::chrono::duration_cast<dbl_milliseconds>(total.time_elapsed);

   std::string label = (_report_level == ReportLevel::Detailed) ? "Total" : " ";

   _stream << fmt::format(text_suite, label, total.passed_count, 
                                             total.failed_count, 
                                             total.skipped_count, 
                                             total.count, 
                                             millis.count());
}

std::string StdOutput::fit_text(const std::string& text, int len)
{
   return (text.size() > len) ? text.substr(0, len - 3) + "..." : text;
}

} // namespace unittest
} // namespace orion
