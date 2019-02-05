//
// TestSuite.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTSUITE_H
#define ORION_UNITTEST_TESTSUITE_H

#include <orion/Common.h>
#include <orion/unittest/Test.h>
#include <orion/unittest/TestOutput.h>
#include <orion/unittest/TestUtils.h>

#include <functional>
#include <string>

namespace orion
{
namespace unittest
{
//---------------------------------------------------------------------------------------

///
/// A test Suite is a set of tests that all share the same fixture.
///
class API_EXPORT TestSuite
{
public:
   explicit TestSuite(const std::string& name) noexcept;
   virtual ~TestSuite() = default;

   DEFAULT_COPY(TestSuite)
   DEFAULT_MOVE(TestSuite)

   /// Name of the test suite
   std::string name() const;
   std::string label() const;
   std::string description() const;

   bool enabled() const;

   std::string disabled_reason() const;

   const TestSuiteStats& stats() const;

   const std::vector<Test>& tests() const;

   uint64_t test_count() const;

   /// Sets up the test suite.
   void setup();

   /// Tears down the test suite.
   void teardown();

   void set_option(option::Label opt);
   void set_option(option::Description opt);
   void set_option(option::SetupFunc opt);
   void set_option(option::TeardownFunc opt);
   void set_option(option::Enabled opt);
   void set_option(option::Disabled opt);

   /// Executes the tests and logs then to output.
   const TestSuiteStats& run_tests(Output& output);

   Test& add_test(Test&& test) noexcept;

   Test& add_test(const std::string& name, TestCaseFunc f) noexcept;

   void add_tests(std::initializer_list<Test> l) noexcept;

private:
   std::string _name;
   std::string _label;
   std::string _description;

   bool _is_enabled;
   std::string _disabled_reason;

   std::function<void()> _setup_func;
   std::function<void()> _teardown_func;

   std::vector<Test> _tests;

   TestSuiteStats _stats;
};

//-------------------------------------------------------------------------------------------------

inline void set_options(TestSuite& /* unused */) noexcept {}

template<typename O>
void set_options(TestSuite& suite, O&& opt) noexcept
{
   suite.set_option(std::forward<decltype(opt)>(opt));
}

template<typename O, typename... Opts>
void set_options(TestSuite& suite, O&& opt, Opts&&... opts) noexcept
{
   set_options(suite, std::forward<decltype(opt)>(opt));
   set_options(suite, std::forward<decltype(opts)>(opts)...);
}

} // namespace unittest
} // namespace orion

#endif /* ORION_UNITTEST_TESTSUITE_H */