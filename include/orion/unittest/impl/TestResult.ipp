//
// TestResult.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTRESULT_IPP
#define ORION_UNITTEST_TESTRESULT_IPP

#include <orion/SourceLocation.h>
#include <orion/TypeTraits.h>
#include <orion/Utils.h>

#include <iostream>
#include <string>

namespace orion
{
namespace unittest
{
//-------------------------------------------------------------------------------------------------

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const Expression& ex)
{
   os << ex.value;
   return os;
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const Actual& a)
{
   os << a.value;
   return os;
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const Expected& e)
{
   os << e.value;
   return os;
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const Message& m)
{
   os << m.value;
   return os;
}

//-------------------------------------------------------------------------------------------------

inline Assertion::Assertion(Result r)
   : _result(r)
{
}

inline Assertion::Assertion(Result r, Expression ex)
   : _result(r)
   , _expression(ex)
{
}

inline Assertion::Assertion(Result r, Expression ex, Actual a, Expected e)
   : Assertion(r, std::move(ex), std::move(a), std::move(e), Message{})
{
}

inline Assertion::Assertion(Result r, Expression ex, Actual a, Expected e, Message m)
   : Assertion(r, std::move(ex), std::move(a), std::move(e), std::move(m), {})
{
}

inline Assertion::Assertion(Result r, Expression ex, Actual a, Expected e, Message m, SourceLocation sl)
   : _result(r)
   , _expression(std::move(ex))
   , _actual(std::move(a))
   , _expected(std::move(e))
   , _msg(std::move(m))
   , _source_location(std::move(sl))
{
}

inline Result Assertion::result() const
{
   return _result;
}

inline Expression Assertion::expression() const
{
   return _expression;
}

inline Actual Assertion::actual() const
{
   return _actual;
}

inline Expected Assertion::expected() const
{
   return _expected;
}

inline Message Assertion::message() const
{
   return _msg;
}

inline SourceLocation Assertion::source_location() const
{
   return _source_location;
}

//-------------------------------------------------------------------------------------------------
// class AssertionPassed

inline AssertionPassed::AssertionPassed()
   : Assertion(Result::Passed)
{
}

inline AssertionPassed::AssertionPassed(Message m)
   : Assertion(Result::Passed, Expression{}, Actual{}, Expected{}, std::move(m))
{
}

//-------------------------------------------------------------------------------------------------
// class AssertionFailed

inline AssertionFailed::AssertionFailed()
   : Assertion(Result::Failed, Expression{}, Actual{}, Expected{}, Message{"Unknown Error"})
{
}

inline AssertionFailed::AssertionFailed(Expression ex, Actual a, Expected e)
   : Assertion(Result::Failed, std::move(ex), std::move(a), std::move(e))
{
}

inline AssertionFailed::AssertionFailed(Expression ex, Actual a, Expected e, Message m)
   : Assertion(Result::Failed, std::move(ex), std::move(a), std::move(e), std::move(m))
{
}

inline AssertionFailed::AssertionFailed(Expression ex,
                                        Actual a,
                                        Expected e,
                                        Message m,
                                        SourceLocation sl)
   : Assertion(Result::Failed,
               std::move(ex),
               std::move(a),
               std::move(e),
               std::move(m),
               std::move(sl))
{
}

inline AssertionFailed::AssertionFailed(Expression ex, Actual a, Expected e, SourceLocation sl)
   : Assertion(Result::Failed, std::move(ex), std::move(a), std::move(e), Message{}, std::move(sl))
{
}

inline AssertionFailed::AssertionFailed(Message m, SourceLocation sl)
   : Assertion(Result::Failed, Expression{}, Actual{}, Expected{}, std::move(m), std::move(sl))
{
}

//-------------------------------------------------------------------------------------------------
// class AssertionException

inline AssertionException::AssertionException()
   : Assertion(Result::Exception, Expression{}, Actual{}, Expected{}, Message{"Unknown exception"})
{
}

inline AssertionException::AssertionException(std::exception_ptr eptr, SourceLocation sl)
   : Assertion(Result::Exception, Expression{}, Actual{}, Expected{}, Message{}, std::move(sl))
   , _eptr(eptr)
{
}

inline AssertionException::AssertionException(std::exception_ptr eptr,
                                              Actual a,
                                              Expected e,
                                              Message m,
                                              SourceLocation sl)
   : Assertion(Result::Exception,
               Expression{},
               std::move(a),
               std::move(e),
               std::move(m),
               std::move(sl))
   , _eptr(eptr)
{
}

inline AssertionException::AssertionException(std::exception_ptr eptr, Message m, SourceLocation sl)
   : Assertion(Result::Exception, Expression{}, Actual{}, Expected{}, std::move(m), std::move(sl))
   , _eptr(eptr)
{
}

//-------------------------------------------------------------------------------------------------
// class AssertionSkipped

inline AssertionSkipped::AssertionSkipped(Message m)
   : Assertion(Result::Skipped, Expression{}, Actual{}, Expected{}, std::move(m))
{
}

inline AssertionSkipped::AssertionSkipped(Message m, SourceLocation sl)
   : Assertion(Result::Skipped, Expression{}, Actual{}, Expected{}, std::move(m), std::move(sl))
{
}

//-------------------------------------------------------------------------------------------------

inline bool TestResult::passed() const
{
   return not failed();
}

inline bool TestResult::failed() const
{
   return _counters.failed > 0;
}

inline bool TestResult::skipped() const
{
   return _counters.total() == _counters.skipped;
}

inline const Counters& TestResult::counters() const
{
   return _counters;
}

inline std::chrono::nanoseconds TestResult::elapsed_time() const
{
   return _time_point_end - _time_point_start;
}

inline const std::vector<Assertion>& TestResult::assertions() const
{
   return _assertions;
}

inline void TestResult::on_start()
{
   _time_point_start = std::chrono::high_resolution_clock::now();
}

inline void TestResult::on_end()
{
   _time_point_end = std::chrono::high_resolution_clock::now();
}

inline void TestResult::log(AssertionPassed&& ap)
{
   ++_counters.passed;

   _assertions.emplace_back(std::move(ap));
}

inline void TestResult::log(AssertionFailed&& af)
{
   ++_counters.failed;

   _assertions.emplace_back(std::move(af));
}

inline void TestResult::log(AssertionException&& ae)
{
   ++_counters.failed;

   _assertions.emplace_back(std::move(ae));
}

inline void TestResult::log(AssertionSkipped&& as)
{
   ++_counters.skipped;

   _assertions.emplace_back(std::move(as));
}

} // namespace unittest
} // namespace orion

#endif // ORION_UNITTEST_TESTRESULT_IPP
