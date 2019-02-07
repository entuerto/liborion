//
// TestSuite.ipp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTSUITE_IPP
#define ORION_UNITTEST_TESTSUITE_IPP

namespace orion
{
namespace unittest
{

//---------------------------------------------------------------------------------------

/// Constructor
inline TestSuite::TestSuite(std::string name) noexcept
   : TestInfo(std::move(name))
{
   _stats.label = label();
}

inline constexpr const TestSuiteStats& TestSuite::stats() const
{
   return _stats;
}

inline constexpr const Tests& TestSuite::tests() const
{
   return _tests;
}

inline constexpr uint64_t TestSuite::test_count() const
{
   return _tests.size();
}

inline void TestSuite::add_test(const Test& test) noexcept
{
   _tests.push_back(&test);
}

/// Executes the tests and logs then to output.
inline const TestSuiteStats& TestSuite::run_tests(Output& output)
{
   output.suite_start(*this);

   invoke_setup_func();

   for (auto test : _tests)
   {
      output.test_start(*test);

      auto& test_result = test->invoke();

      if (test_result.skipped())
      {
         _stats.tests.skipped++;
      }
      else if (test_result.failed())
      {
         _stats.tests.failed++;
      }
      else
      {
         _stats.tests.passed++;
      }

      _stats.assertions += test_result.counters();
      _stats.time_elapsed += test_result.elapsed_time();

      output.test_end(test_result);
   }

   invoke_teardown_func();

   output.suite_end(_stats);

   return _stats;
}

} // namespace unittest
} // namespace orion

#endif // ORION_UNITTEST_TESTSUITE_IPP
