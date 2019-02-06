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

#include <clara/clara.hpp>

#include <iostream>

namespace orion
{
namespace unittest
{

constexpr uint32_t TestRunner::test_suite_count() const
{
   return _test_suites.size();
}

uint32_t TestRunner::test_case_count() const
{
   uint32_t count = 0;

   for (auto& item : _test_suites)
   {
      count += item.test_count();
   }

   return count;
}

bool TestRunner::parse(int argc, char* argv[])
{
   using namespace clara;

   bool show_help = false;

   auto options = Help(show_help)
                | Opt(_level, "level")["-l"]("Set report output level"); 

   try
   {
      auto result = options.parse(Args(argc, argv));
      if (not result)
      {
         std::cerr << "Error: \n" << result.errorMessage() << "\n\n";
         
         show_help = true;
      }
   }
   catch(const std::exception& e)
   {
      std::cerr << "Error: \n" << e.what() << "\n\n";

      show_help = true;
   }

   if (show_help)
   {
      options.writeToStream(std::cout);
      return false;
   }

   return true;
}

TestSuite& TestRunner::add_test_suite(TestSuite&& suite)
{
   _test_suites.push_back(std::move(suite));
   return _test_suites.back();
}

bool TestRunner::run(int argc, char* argv[])
{
   if (not parse(argc, argv))
      return false;

   StdOutput output(std::cout, _level);

   return run_tests(output);
}

bool TestRunner::run_tests(Output& output, const std::string& /*suite_name*/ /* = "" */)
{
   Totals totals{};

   output.runner_start(*this);

   auto start = std::chrono::high_resolution_clock::now();

   for (auto& item : _test_suites)
   {
      auto& suite_stats = item.run_tests(output);

      totals.assertions += suite_stats.assertions;
      totals.tests      += suite_stats.tests;
   }

   auto end = std::chrono::high_resolution_clock::now();

   totals.time_elapsed = end - start;

   output.runner_end(totals);
   return totals.tests.failed == 0;
}

TestRunner& TestRunner::runner()
{
   static TestRunner s_runner;

   return s_runner;
}

} // namespace unittest
} // namespace orion
