// Test.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
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

//---------------------------------------------------------------------------------------

///
///
///
class API_EXPORT Test
{
public:
   // NO_COPY(Test)

   Test(const std::string& name, const Suite& suite);
   Test(const std::string& name, const Suite& suite, TestCaseFunc&& f);
   virtual ~Test();

   std::string name() const;
   std::string label() const;
   std::string description() const;

   bool enabled() const;

   std::string disabled_reason() const;

   const TestResult& test_result() const;

   // Sets up the test fixture.
   void setup();

   // Tears down the test fixture.
   void teardown();

   TestCaseFunc& case_func();

   TestResult& execute_test();

   void set_option(option::Label opt);
   void set_option(option::Description opt);
   void set_option(option::SetupFunc opt);
   void set_option(option::TeardownFunc opt);
   void set_option(option::Enabled opt);
   void set_option(option::Disabled opt);

   template<class Func, typename T, typename... Args>
   bool assert(const T& expected, const T& actual, Args... args);

   template<bool ExpectedValue, typename... Args>
   bool assert(const bool value, Args... args);

   template<typename ExpectedException, typename Func, typename... Args>
   bool assert_throw(Func f, Args... args);

   template<typename... Args>
   void fail(Args... args);

   template<typename... Args>
   void fail_if(const bool value, Args... args);

protected:
   virtual void do_execute_test() const;

private:
   std::string _name;
   std::string _label;
   std::string _description;

   bool _is_enabled;
   std::string _disabled_reason;

   TestResult _test_result;

   std::function<void()> _setup_func;
   std::function<void()> _teardown_func;

   TestCaseFunc _func;
};

template<class Func, typename T, typename... Args>
bool Test::assert(const T& expected, const T& actual, Args... args)
{
   Func cmp;

   if (cmp(expected, actual))
   {
      _test_result.log_success();
      return true;
   }

   _test_result.log_failure(expected, actual, args...);

   return false;
}

template<bool ExpectedValue, typename... Args>
bool Test::assert(const bool value, Args... args)
{
   if (value == ExpectedValue)
   {
      _test_result.log_success();
      return true;
   }

   _test_result.log_failure(ExpectedValue, value, args...);

   return false;
}

template<typename ExpectedException, typename Func, typename... Args>
bool Test::assert_throw(Func f, Args... args)
{
   try
   {
      f();
   }
   catch (const ExpectedException&)
   {
      _test_result.log_success();
      return true;
   }
   catch (const std::exception& e)
   {
      _test_result.log_exception(e, "An unexpected exception was thrown: ", args...);
      return false;
   }
   catch (...)
   {
      _test_result.log_exception(
         std::current_exception(), "An unexpected, unknown exception was thrown: ", args...);
      return false;
   }
   // Not thrown
   fail("The exception was not thrown: ", type_name<ExpectedException>(), args...);
   return false;
}

template<typename... Args>
void Test::fail(Args... args)
{
   _test_result.log_failure(args...);
}

template<typename... Args>
void Test::fail_if(const bool value, Args... args)
{
   if (not value)
   {
      _test_result.log_success();
      return;
   }

   _test_result.log_failure(false, value, args...);

   return;
}

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

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TEST_H */
