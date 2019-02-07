//
// TestResult.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTRESULT_H
#define ORION_UNITTEST_TESTRESULT_H

#include <orion/Common.h>
#include <orion/Chrono.h>
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

   // Convertible to string
   template <typename T>
   explicit Expression(T v)
      : value(static_cast<std::string>(v)) {}

   constexpr bool empty() const { return value.empty(); }

   constexpr explicit operator const std::string&() const  { return value; }
   constexpr explicit operator std::string&()              { return value; }   
};

struct Actual
{
   std::string value;

   Actual() = default;
   explicit Actual(std::string v)
      : value(std::move(v)) {}

   // Convertible to string
   template <typename T>
   explicit Actual(T v)
      : value(static_cast<std::string>(v)) {}

   constexpr bool empty() const { return value.empty(); }

   constexpr explicit operator const std::string&() const  { return value; }
   constexpr explicit operator std::string&()              { return value; }   
};

struct Expected
{
   std::string value;

   Expected() = default;
   explicit Expected(std::string v)
      : value(std::move(v)) {}

   // Convertible to string
   template <typename T>
   explicit Expected(T v)
      : value(static_cast<std::string>(v)) {}

   constexpr bool empty() const { return value.empty(); }

   constexpr explicit operator const std::string&() const  { return value; }
   constexpr explicit operator std::string&()              { return value; }   
};

struct Message
{
   std::string value;

   Message() = default;
   explicit Message(std::string v)
      : value(std::move(v)) {}

   // Convertible to string
   template <typename T>
   explicit Message(T v)
      : value(static_cast<std::string>(v)) {}

   constexpr bool empty() const { return value.empty(); }

   constexpr explicit operator const std::string&() const  { return value; }
   constexpr explicit operator std::string&()              { return value; }
};

class Assertion
{
public:
   explicit Assertion(Result r);
   Assertion(Result r, Expression ex);
   Assertion(Result r, Expression ex, Actual a, Expected e);
   Assertion(Result r, Expression ex, Actual a, Expected e, Message m);
   Assertion(Result r, Expression ex, Actual a, Expected e, Message m, SourceLocation sl);

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
   explicit AssertionPassed(Message m);
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
   AssertionException(const std::exception_ptr& eptr, SourceLocation sl);
   AssertionException(const std::exception_ptr& eptr, Actual a, Expected e, Message m, SourceLocation sl);
   AssertionException(const std::exception_ptr& eptr, Message m, SourceLocation sl);

private:
   std::exception_ptr _eptr;
};

class AssertionSkipped : public Assertion
{
public:
   explicit AssertionSkipped(Message m);
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
   Counters _counters{};

   HighResTimePoint _time_point_start;
   HighResTimePoint _time_point_end;

   std::vector<Assertion> _assertions;
};

} // namespace unittest
} // namespace orion

#include <orion/unittest/impl/TestResult.ipp>

#endif /* ORION_UNITTEST_TESTRESULT_H */
