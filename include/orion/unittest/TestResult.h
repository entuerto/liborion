//
// TestResult.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTRESULT_H
#define ORION_UNITTEST_TESTRESULT_H

#include <orion/Orion-Stddefs.h>
#include <orion/Chrono.h>
#include <orion/SourceLocation.h>
#include <orion/Utils.h>

#include <orion/unittest/TestUtils.h>

#include <chrono>
#include <string>
#include <vector>

namespace orion
{
namespace unittest
{

//-------------------------------------------------------------------------------------------------

enum class Result
{
   Passed,
   Failed,
   Exception,
   Skipped
};

//-------------------------------------------------------------------------------------------------

struct API_EXPORT Assertion
{
   Result result;

   std::string msg;
   std::string expected;
   std::string actual;

   SourceLocation src_loc;
};

///
/// Holds the results of a test case.
///
class API_EXPORT TestResult
{
public:
   TestResult();
   ~TestResult() = default;

   DEFAULT_COPY(TestResult)
   DEFAULT_MOVE(TestResult)

   bool passed() const;
   bool failed() const;
   bool skipped() const;

   const Counters& counters() const;

   std::chrono::nanoseconds time_elapsed() const;

   const std::vector<Assertion>& assertions() const;

   void on_start();
   void on_end();

   template<typename... Args>
   void log_success(Args... args);

   template<typename T, typename... Args>
   void log_failure(const T& expected, const T& actual, Args... args);

   template<typename... Args>
   void log_failure(Args... args);

   template<typename E, typename... Args>
   void log_exception(const E& e, Args... args);

   template<typename... Args>
   void log_exception(std::exception_ptr eptr, Args... args);

   template<typename... Args>
   void log_skipped(Args... args);

private:
   Counters _counters;

   HighResTimePoint _time_point_start;
   HighResTimePoint _time_point_end;

   std::vector<Assertion> _assertions;
};

} // namespace unittest
} // namespace orion

#include <orion/unittest/impl/TestResult.ipp>

#endif /* ORION_UNITTEST_TESTRESULT_H */
