//
// TestResult.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTRESULT_IPP
#define ORION_UNITTEST_TESTRESULT_IPP

#include <orion/Utils.h>

#include <string>

namespace orion
{
namespace unittest
{

struct StringConcat
{
   std::string& text;

   template<typename T>
   void operator()(T /*unused*/)
   {
   }

   void operator()(const char* v) { text += v; }

   void operator()(const std::string& v) { text += v; }
};

template<typename... Args>
void TestResult::log_success(Args... args)
{
   ++_counters.passed;

   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   _assertions.push_back(
      Assertion{Result::Passed, msg, "", "", get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename T, typename... Args>
void TestResult::log_failure(const T& expected, const T& actual, Args... args)
{
   ++_counters.failed;

   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   _assertions.push_back(Assertion{Result::Failed,
                                   msg,
                                   get_as_string(expected),
                                   get_as_string(actual),
                                   get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename... Args>
void TestResult::log_failure(Args... args)
{
   ++_counters.failed;

   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   _assertions.push_back(
      Assertion{Result::Failed, msg, "", "", get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename E, typename... Args>
void TestResult::log_exception(const E& e, Args... args)
{
   ++_counters.failed;

   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   msg += type_name(e) + " (" + e.what() + ") ";

   _assertions.push_back(
      Assertion{Result::Exception, msg, "", "", get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename... Args>
void TestResult::log_exception(std::exception_ptr eptr, Args... args)
{
   ++_counters.failed;

   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   msg += type_name(std::move(eptr));

   _assertions.push_back(
      Assertion{Result::Exception, msg, "", "", get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename... Args>
void TestResult::log_skipped(Args... args)
{
   ++_counters.skipped;

   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   _assertions.push_back(
      Assertion{Result::Skipped, msg, "", "", get_value<SourceLocation>(t, SourceLocation{})});
}

} // namespace unittest
} // namespace orion

#endif // ORION_UNITTEST_TESTRESULT_IPP
