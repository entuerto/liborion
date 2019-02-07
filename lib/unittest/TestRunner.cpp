//
// TestRunner.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/unittest/TestRunner.h>

#include <orion/String.h>
#include <orion/unittest/TestBase.h>
#include <orion/unittest/TestOutput.h>
#include <orion/unittest/TestStdOutput.h>

#include <clara/clara.hpp>

#include <iostream>

using namespace std::string_literals;

namespace orion
{
namespace unittest
{

namespace detail
{
   
std::string fit_text(const std::string& text, std::size_t len)
{
   return (text.size() > len) ? text.substr(0, len - 3) + "..." : text;
}

std::string desc_text(const TestSuite& s, std::size_t len)
{
   std::string desc = fit_text(s.description(), len);

   return desc.empty() ? "None" : desc;
}

std::string status_text(const TestSuite& s, std::size_t /* len */)
{

   return fmt::format("Tests({}), {}",
                      s.test_count(),
                      (s.enabled() ? "Enabled" : "Disabled: "s + s.disabled().value));
}

} // namespace detail


constexpr uint32_t TestRunner::test_suite_count() const
{
   return _test_suites.size();
}

uint32_t TestRunner::test_case_count() const
{
   uint32_t count = 0;

   for (auto& item : _test_suites)
   {
      if (filter_test_suite(item.label()))
         continue;
         
      count += item.test_count();
   }

   return count;
}

bool TestRunner::parse(int argc, char* argv[])
{
   using namespace clara;

   const auto verbosity = [&](const std::string& v) {
      if (iequals(v, "error"s))
         _config.level = ReportLevel::Error;
      else if (iequals(v, "normal"s))
         _config.level = ReportLevel::Short;
      else if (iequals(v, "high"s))
         _config.level = ReportLevel::Detailed;
      else
         return ParserResult::runtimeError("Unrecognised verbosity, '" + v + "'");
      return ParserResult::ok(ParseResultType::Matched);
   };

   bool show_help = false;

   auto options = Help(show_help)
         | Opt(_config.list_tests)["-l"]["--list-tests"]("list all tests")
         | Opt(_config.suite_name, "name")["-n"]["--name"]("suite name")
         | Opt(verbosity, "error|normal|high")["-v"]["--verbosity"]("set output verbosity");

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

   if (_config.list_tests)
   {
      print_tests_suites();
      return true;
   }

   StdOutput output(std::cout, _config.level);

   return run_tests(output);
}

bool TestRunner::run_tests(Output& output)
{
   Totals totals{};

   output.runner_start(*this);

   auto start = std::chrono::high_resolution_clock::now();

   for (auto& item : _test_suites)
   {
      if (filter_test_suite(item.label()))
         continue;

      auto& suite_stats = item.run_tests(output);

      totals.assertions += suite_stats.assertions;
      totals.tests      += suite_stats.tests;
   }

   auto end = std::chrono::high_resolution_clock::now();

   totals.time_elapsed = end - start;

   output.runner_end(totals);
   return totals.tests.failed == 0;
}

void TestRunner::print_tests_suites()
{
   std::cout << "Unit tests" << '\n' << '\n';

   std::cout << fmt::format(" {0:<25}  {1:<50} {2}\n", "Test Suites", "Description", "Status");

   try
   {
      for (const auto& item : _test_suites)
      {
         std::cout << fmt::format("  {0:<25} {1:<50} {2}\n", 
                                  detail::fit_text(item.label(), 25), 
                                  detail::desc_text(item, 50),
                                  detail::status_text(item, 20));
      }
   }
   catch (const std::exception& e)
   {
      std::cerr << e.what() << '\n';
   }
   std::cout << '\n';
}

bool TestRunner::filter_test_suite(const std::string& name) const
{
   if (_config.suite_name.empty())
      return false;

   return not iequals(_config.suite_name, name);
}

TestRunner& TestRunner::runner()
{
   static TestRunner s_runner;

   return s_runner;
}

} // namespace unittest
} // namespace orion
