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
#include <orion/unittest/TestBase.h>
#include <orion/unittest/TestOutput.h>
#include <orion/unittest/TestUtils.h>

#include <string>
#include <vector>

namespace orion
{
namespace unittest
{
//---------------------------------------------------------------------------------------

///
/// A test Suite is a set of tests that all share the same fixture.
///
class TestSuite : public TestInfo
{
public:
   explicit TestSuite(std::string name) noexcept;
   ~TestSuite() override = default;

   DEFAULT_COPY(TestSuite)
   DEFAULT_MOVE(TestSuite)

   const TestSuiteStats& stats() const;

   const std::vector<Test>& tests() const;

   constexpr uint64_t test_count() const;

   /// Executes the tests and logs then to output.
   const TestSuiteStats& run_tests(Output& output);

   Test& add_test(Test&& test) noexcept;

   Test& add_test(const std::string& name, TestCaseFunc f) noexcept;

   void add_tests(std::initializer_list<Test> l) noexcept;

private:
   std::vector<Test> _tests;

   TestSuiteStats _stats;
};

} // namespace unittest
} // namespace orion

#include <orion/unittest/impl/TestSuite.ipp>

#endif // ORION_UNITTEST_TESTSUITE_H 