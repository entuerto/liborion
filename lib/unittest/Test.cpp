//
// Test.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/unittest/Test.h>

#include <orion/unittest/TestResult.h>
#include <orion/unittest/TestSuite.h>

#include <algorithm>
#include <exception>
#include <map>

namespace orion
{
namespace unittest
{

using namespace orion::unittest::option;

//---------------------------------------------------------------------------------------

Test::Test(const std::string& name, const TestSuite& suite)
   : _name(name)
   , _label(name)
   , _is_enabled(true)
   , _test_result()
{
}

Test::Test(const std::string& name, const TestSuite& suite, TestCaseFunc&& f)
   : _name(name)
   , _label(name)
   , _is_enabled(true)
   , _test_result()
   , _func(std::move(f))
{
}

Test::~Test() = default;

std::string Test::name() const
{
   return _name;
}

std::string Test::label() const
{
   return _label;
}

std::string Test::description() const
{
   return _description;
}

const TestResult& Test::test_result() const
{
   return _test_result;
}

void Test::set_option(Label opt)
{
   _label = opt.text;
}

void Test::set_option(Description opt)
{
   _description = opt.text;
}

void Test::set_option(SetupFunc opt)
{
   _setup_func = std::move(opt.func);
}

void Test::set_option(TeardownFunc opt)
{
   _teardown_func = std::move(opt.func);
}

void Test::set_option(Enabled /*unused*/)
{
   _is_enabled = true;
}

void Test::set_option(Disabled opt)
{
   _is_enabled      = false;
   _disabled_reason = std::move(opt.reason);
}

bool Test::enabled() const
{
   return _is_enabled;
}

std::string Test::disabled_reason() const
{
   return _disabled_reason;
}

void Test::setup() const
{
   if (_setup_func)
      _setup_func();
}

void Test::teardown() const
{
   if (_teardown_func)
      _teardown_func();
}

TestCaseFunc& Test::case_func()
{
   return _func;
}

void Test::do_invoke() const
{
   if (_func)
      _func(*const_cast<Test*>(this));
}

const TestResult& Test::invoke() const
{
   if (not enabled())
   {
      _test_result.log(AssertionSkipped{Message{_disabled_reason}});
      return _test_result;
   }

   _test_result.on_start();
   try
   {
      setup();
      do_invoke();
      teardown();
   }
   catch (const std::exception& e)
   {
      std::string msg{"An unexpected exception was thrown: "};

      msg += e.what();

      _test_result.log(AssertionException{std::make_exception_ptr(e), Message{msg}, _src_loc});
   }
   catch (...)
   {
      std::string msg{"An unexpected, unknown exception was thrown: "};

      auto cur_exp = std::current_exception();

      msg += type_name(cur_exp);

      _test_result.log(AssertionException{cur_exp, Message{msg}, _src_loc});
   }
   _test_result.on_end();

   return _test_result;
}

} // namespace unittest
} // namespace orion
