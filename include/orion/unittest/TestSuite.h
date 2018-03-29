//
// TestSuite.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTSUITE_H
#define ORION_UNITTEST_TESTSUITE_H

#include <orion/Orion-Stddefs.h>
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
/// A test suite is a set of tests that all share the same fixture.
///
class API_EXPORT Suite
{
public:
   Suite(const std::string& name);
   virtual ~Suite() = default;

   /// Name of the test suite
   std::string name() const;
   std::string label() const;
   std::string description() const;

   bool enabled() const;

   std::string disabled_reason() const;

   const Stats& stats() const;

   const std::vector<Test>& test_cases() const;

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
   const Stats& run_tests(Output& output);

   Test& add_test(Test&& test);

   Test& add_test(const std::string& name, TestCaseFunc f);

   void add_tests(std::initializer_list<Test> l);

private:
   std::string _name;
   std::string _label;
   std::string _description;

   bool _is_enabled;
   std::string _disabled_reason;

   std::function<void()> _setup_func;
   std::function<void()> _teardown_func;

   std::vector<Test> _test_cases;

   Stats _stats;
};

//-------------------------------------------------------------------------------------------------

inline void set_options(Suite& /* suite */) {}

template<typename O>
void set_options(Suite& suite, O&& opt)
{
   suite.set_option(std::forward<decltype(opt)>(opt));
}

template<typename O, typename... Opts>
void set_options(Suite& suite, O&& opt, Opts&&... opts)
{
   set_options(suite, std::forward<decltype(opt)>(opt));
   set_options(suite, std::forward<decltype(opts)>(opts)...);
}

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TESTSUITE_H */
