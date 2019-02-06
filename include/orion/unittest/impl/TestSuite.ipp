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
   , _stats()
{
   _stats.label = label();
}

inline const TestSuiteStats& TestSuite::stats() const
{
   return _stats;
}

inline const std::vector<Test>& TestSuite::tests() const
{
   return _tests;
}

inline constexpr uint64_t TestSuite::test_count() const
{
   return _tests.size();
}

inline Test& TestSuite::add_test(const std::string& name, TestCaseFunc f) noexcept
{
   Test t(name, std::move(f));

   return add_test(std::move(t));
}

inline Test& TestSuite::add_test(Test&& test) noexcept
{
   _tests.push_back(std::move(test));
   return _tests.back();
}

inline void TestSuite::add_tests(std::initializer_list<Test> l) noexcept
{
   _tests.insert(_tests.end(), l);
}

/// Executes the tests and logs then to output.
inline const TestSuiteStats& TestSuite::run_tests(Output& output)
{
   output.suite_start(*this);

   invoke_setup_func();

   for (auto& test : _tests)
   {
      output.test_start(test);

      auto& test_result = test.invoke();

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
