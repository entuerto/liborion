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

struct Expression
{
   std::string value;

   Expression() = default;
   explicit Expression(std::string v)
      : value(std::move(v)) {}

   constexpr bool empty() const { return value.empty(); }
};

struct Actual
{
   std::string value;

   Actual() = default;
   explicit Actual(std::string v)
      : value(std::move(v)) {}

   constexpr bool empty() const { return value.empty(); }
};

struct Expected
{
   std::string value;

   Expected() = default;
   explicit Expected(std::string v)
      : value(std::move(v)) {}

   constexpr bool empty() const { return value.empty(); }
};

struct Message
{
   std::string value;

   Message() = default;
   explicit Message(std::string v)
      : value(std::move(v)) {}

   constexpr bool empty() const { return value.empty(); }
};

class Assertion
{
public:
   Assertion(Result r);
   Assertion(Result r, Expression ex);
   Assertion(Result r, Expression ex, Actual a, Expected e);
   Assertion(Result r, Expression ex, Actual a, Expected e, Message m);
   Assertion(Result r, Expression ex, Actual a, Expected e, Message m, SourceLocation sl);
   virtual ~Assertion() = default;

   Result result() const;
   Expression expression() const;
   Actual actual() const;
   Expected expected() const;
   Message message() const;
   SourceLocation source_location() const;

private:
   Result _result;

   Expression _expression;
   Actual _actual;
   Expected _expected;
   Message _msg;

   SourceLocation _source_location;
};

class AssertionPassed : public Assertion
{
public:
   AssertionPassed();
   AssertionPassed(Message m);
};

class AssertionFailed : public Assertion
{
public:
   AssertionFailed();
   AssertionFailed(Expression ex, Actual a, Expected e);
   AssertionFailed(Expression ex, Actual a, Expected e, Message m);
   AssertionFailed(Expression ex, Actual a, Expected e, Message m, SourceLocation sl);
   AssertionFailed(Expression ex, Actual a, Expected e, SourceLocation sl);
   AssertionFailed(Message m, SourceLocation sl);
};

class AssertionException : public Assertion
{
public:
   AssertionException();
   AssertionException(std::exception_ptr eptr, SourceLocation sl);
   AssertionException(std::exception_ptr eptr, Actual a, Expected e, Message m, SourceLocation sl);
   AssertionException(std::exception_ptr eptr, Message m, SourceLocation sl);

private:
   std::exception_ptr _eptr;
};

class AssertionSkipped : public Assertion
{
public:
   AssertionSkipped(Message m);
   AssertionSkipped(Message m, SourceLocation sl);
};

//-------------------------------------------------------------------------------------------------

///
/// Holds the results of a test case.
///
class TestResult
{
public:
   TestResult() = default;

   bool passed() const;
   bool failed() const;
   bool skipped() const;

   const Counters& counters() const;

   std::chrono::nanoseconds elapsed_time() const;

   const std::vector<Assertion>& assertions() const;

   void on_start();
   void on_end();

   void log(AssertionPassed&& ap);
   void log(AssertionFailed&& af);
   void log(AssertionException&& ae);
   void log(AssertionSkipped&& as);

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
