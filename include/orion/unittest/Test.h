//
// Test.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TEST_H
#define ORION_UNITTEST_TEST_H

#include <orion/Orion-Stddefs.h>
#include <orion/Utils.h>
#include <orion/unittest/TestOptions.h>
#include <orion/unittest/TestOutput.h>
#include <orion/unittest/TestResult.h>

#include <functional>
#include <string>

namespace orion
{
namespace unittest
{
// Forward declaration
class Test;

using TestCaseFunc = std::function<void(Test&)>;

enum
{
   lt, // Less then
   le, // Less equal then
   eq, // Equals
   ne, // Not equals
   ge, // Greater equals then
   gt  // Greater then
};

//---------------------------------------------------------------------------------------

///
///
///
class API_EXPORT Test
{
public:
   // NO_COPY(Test)

   Test(const std::string& name, const TestSuite& suite);
   Test(const std::string& name, const TestSuite& suite, TestCaseFunc&& f);
   virtual ~Test();

   std::string name() const;
   std::string label() const;
   std::string description() const;

   bool enabled() const;

   std::string disabled_reason() const;

   const TestResult& test_result() const;

   // Sets up the test fixture.
   void setup() const;

   // Tears down the test fixture.
   void teardown() const;

   TestCaseFunc& case_func();

   const TestResult& invoke() const;

   void set_option(option::Label opt);
   void set_option(option::Description opt);
   void set_option(option::SetupFunc opt);
   void set_option(option::TeardownFunc opt);
   void set_option(option::Enabled opt);
   void set_option(option::Disabled opt);

   template<class Func, typename T, typename... Args>
   void xassert(const T& expected, const T& actual, Args... args);

   template<int op, typename T, typename... Args>
   void xassert(const T& expected, const T& actual, Args... args);

   template<bool ExpectedValue, typename... Args>
   void xassert(bool value, Args... args);

   template<typename... Args>
   void fail(Args... args);

   template<typename... Args>
   void fail_if(bool value, Args... args);

   //
   // Exception handling
   // 

   template<typename... Args>
   void expected_exception_not_thrown(Args... args);

   template<typename ExceptionT, typename... Args>
   void exception_thrown_as_expected(const ExceptionT& e, Args... args);

   template<typename... Args>
   void exception_thrown_as_expected(std::exception_ptr eptr, Args... args);

   template<typename ExceptionT, typename... Args>
   void unexpected_exception_thrown(const ExceptionT& e, Args... args);

   template<typename... Args>
   void unexpected_exception_thrown(std::exception_ptr eptr, Args... args);

protected:
   virtual void do_invoke() const;

private:
   std::string _name;
   std::string _label;
   std::string _description;

   bool _is_enabled;
   std::string _disabled_reason;

   mutable TestResult _test_result;

   std::function<void()> _setup_func;
   std::function<void()> _teardown_func;

   TestCaseFunc _func;
};

//---------------------------------------------------------------------------------------

inline void set_options(Test& /* test */)
{
}

template<typename O>
void set_options(Test& test, O&& opt)
{
   test.set_option(std::forward<decltype(opt)>(opt));
}

template<typename O, typename... Opts>
void set_options(Test& test, O&& opt, Opts&&... opts)
{
   set_options(test, std::forward<decltype(opt)>(opt));
   set_options(test, std::forward<decltype(opts)>(opts)...);
}

} // namespace unittest
} // namespace orion

#include <orion/unittest/impl/Test.ipp>

#endif /* ORION_UNITTEST_TEST_H */
