//
// Test.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTBASE_H
#define ORION_UNITTEST_TESTBASE_H

#include <orion/Common.h>

#include <chrono>
#include <functional>
#include <string>

namespace orion
{
namespace unittest
{
//-------------------------------------------------------------------------------------------------
// Options for tests

template<typename T, typename Tag>
struct Option
{
   T value;

   constexpr operator T() const noexcept { return value; }
};


using Label        = Option<std::string, struct LabelTag>;
using Description  = Option<std::string, struct DescriptionTag>;
using SetupFunc    = Option<std::function<void()>, struct SetupFuncTag>;
using TeardownFunc = Option<std::function<void()>, struct TeardownFuncTag>;
using Enabled      = Option<bool, struct EnabledTag>;
using Disabled     = Option<std::string, struct DisabledTag>;

struct Timeout
{
   std::chrono::milliseconds ms;

   constexpr operator std::chrono::milliseconds() const noexcept { return ms; }
};

//-------------------------------------------------------------------------------------------------
// TestInfo 

class TestInfo
{
public:
   DEFAULT_COPY(TestInfo)
   DEFAULT_MOVE(TestInfo)

   explicit TestInfo(std::string name);
   virtual ~TestInfo() = default;

   /// Return the name of the test
   std::string name() const;
   /// Returns the label of the test
   Label label() const;
   /// Returns the description of the test
   Description description() const;
   /// Returns the enabled status of the test
   Enabled enabled() const;
   /// Returns the disabled reason (text) of the test
   Disabled disabled() const;

   void set_option(Label opt);
   void set_option(Description opt);
   void set_option(SetupFunc opt);
   void set_option(TeardownFunc opt);
   void set_option(Enabled opt);
   void set_option(Disabled opt);

   /// Calls the set up function for a test
   void invoke_setup_func() const;
   /// Calls the teardown function for a test
   void invoke_teardown_func() const;

private:
   std::string _name;

   Label _label;
   Description _description;
   Enabled _enabled{true};
   Disabled _disabled;

   SetupFunc _setup_func;
   TeardownFunc _teardown_func;
};

//-------------------------------------------------------------------------------------------------

inline void set_options(TestInfo& /* unused */)
{
}

template<typename O>
void set_options(TestInfo& t, O&& opt)
{
   t.set_option(std::forward<decltype(opt)>(opt));
}

template<typename O, typename... Opts>
void set_options(TestInfo& t, O&& opt, Opts&&... opts)
{
   set_options(t, std::forward<decltype(opt)>(opt));
   set_options(t, std::forward<decltype(opts)>(opts)...);
}

} // namespace unittest
} // namespace orion

#include <orion/unittest/impl/TestBase.ipp>

#endif // ORION_UNITTEST_TESTBASE_H