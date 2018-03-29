//
// TestRunner.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/unittest/TestRunner.h>

#include <orion/unittest/TestOutput.h>
#include <orion/unittest/TestStdOutput.h>

#include <boost/program_options.hpp>

#include <iostream>

namespace opts = boost::program_options;

namespace orion
{
namespace unittest
{

Runner::Runner()
   : _level(ReportLevel::Detailed)
{
}

uint32_t Runner::test_suite_count() const
{
   return _test_suites.size();
}

uint32_t Runner::test_case_count() const
{
   uint32_t count = 0;

   for (auto& item : _test_suites)
   {
      count += item.test_count();
   }

   return count;
}

bool Runner::parse(int argc, char* argv[])
{
   opts::options_description desc("Test options");

   desc.add_options()("help,h", "Help message")(
      "level,l",
      opts::value<ReportLevel>(&_level)->default_value(ReportLevel::Error),
      "Set report output level");

   opts::variables_map vm;
   opts::store(opts::parse_command_line(argc, argv, desc), vm);
   opts::notify(vm);

   if (vm.count("help") != 0u)
   {
      std::cout << desc << "\n";
      return false;
   }

   return true;
}

Suite& Runner::add_test_suite(Suite&& suite)
{
   _test_suites.push_back(std::move(suite));
   return _test_suites.back();
}

bool Runner::run(int argc, char* argv[])
{
   if (not parse(argc, argv))
      return false;

   StdOutput output(std::cout, _level);

   return run_tests(output);
}

bool Runner::run_tests(Output& output, const std::string& /*suite_name*/ /* = "" */)
{
   Stats stats{};

   stats.count = test_case_count();

   output.write_header(stats.count);

   auto start = std::chrono::high_resolution_clock::now();

   for (auto& item : _test_suites)
   {
      auto suite_stats = item.run_tests(output);

      stats.passed_count += suite_stats.passed_count;
      stats.failed_count += suite_stats.failed_count;
      stats.skipped_count += suite_stats.skipped_count;

      stats.item_count += suite_stats.item_count;
      stats.passed_item_count += suite_stats.passed_item_count;
      stats.failed_item_count += suite_stats.failed_item_count;
      stats.skipped_item_count += suite_stats.skipped_item_count;

      stats.time_elapsed += suite_stats.time_elapsed;
   }

   auto end = std::chrono::high_resolution_clock::now();

   stats.time_elapsed = end - start;

   output.write_footer(stats);
   return stats.failed_count == 0;
}

Runner& Runner::runner()
{
   static Runner s_runner;

   return s_runner;
}

} // namespace unittest
} // namespace orion
