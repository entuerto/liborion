// Evaluators.h
//
// Copyright 2010 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.
//
#ifndef ORION_UNITTEST_EVALUATORS_H
#define ORION_UNITTEST_EVALUATORS_H

#include <string>
#include <sstream>
#include <iomanip>

#include <orion/Orion-Stddefs.h>
#include <orion/unittest/TestResult.h>

#include <boost/format.hpp>

#define TEST_CMP_PRED2(NAME, OPERATOR) \
template <typename T1, typename T2>    \
struct NAME                            \
{                                      \
   static const std::string op;        \
   std::string expr1;                  \
   T1 val1;                            \
   std::string expr2;                  \
   T2 val2;                            \
\
   operator bool() const { return val1 OPERATOR val2; } \
}; \
template <typename T1, typename T2> \
const std::string NAME<T1, T2>::op = #OPERATOR;

namespace orion
{
namespace unittest
{

TEST_CMP_PRED2(Equals, ==);
TEST_CMP_PRED2(NotEquals, !=);
TEST_CMP_PRED2(Less, <);
TEST_CMP_PRED2(LessEquals, <=);
TEST_CMP_PRED2(Greater, >);
TEST_CMP_PRED2(GreaterEquals, >=);

//---------------------------------------------------------------------------------------

template <typename T>
bool equals(const T& val1, const T& val2)
{
   return val1 == val2;
}

template <typename T>
bool equals_true(const T& val1)
{
   return val1;
}

template <typename T>
bool equals_false(const T& val1)
{
   return not val1;
}

//---------------------------------------------------------------------------------------

template <class T, std::size_t N>
std::ostream& operator<<(std::ostream& o, const std::array<T, N>& arr)
{
   o << "[ "
     << std::setbase(16)
     << std::showbase;

   for (const auto& b : arr)
      o << static_cast<uint64_t>(b) << " ";

   o << "]";
   return o;
}

//---------------------------------------------------------------------------------------

template <typename P>
bool evaluate(P predicate, TestResult* test_result, const std::string& file, int line)
{
   try
   {
      if (predicate)
      {
         test_result->on_success(TestResultItem::create_success("", file, line));
         return true;
      }

      std::ostringstream expected;
      std::ostringstream got;

      expected << predicate.expr1 << " " << predicate.op << " " << predicate.expr2;

      got << predicate.val1 << ", " << predicate.val2;

      test_result->on_failure(TestResultItem::create_failure("", expected.str(), got.str(), file, line));
   }
   catch (...)
   {
      auto item = TestResultItem::create_failure("Unhandled exception", file, line);
      test_result->on_failure(std::move(item));
   }
   return false;
}

//---------------------------------------------------------------------------------------

inline bool evaluate_true(bool eval, const std::string& expr, TestResult* test_result, const std::string& file, int line)
{
   try
   {
      if (eval)
      {
         test_result->on_success(TestResultItem::create_success("", file, line));
         return true;
      }
      std::string expected = boost::str(boost::format("%s evaluates to true") % expr);
      std::string got = boost::str(boost::format("%s evaluates to false") % expr);

      test_result->on_failure(TestResultItem::create_failure("", expected, got, file, line));
   }
   catch (...)
   {
      auto item = TestResultItem::create_failure("Unhandled exception", file, line);
      test_result->on_failure(std::move(item));
   }
   return false;
}

//---------------------------------------------------------------------------------------

inline bool evaluate_false(bool eval, const std::string& expr, TestResult* test_result, const std::string& file, int line)
{
   try
   {
      if (not eval)
      {
         test_result->on_success(TestResultItem::create_success("", file, line));
         return true;
      }
      std::string expected = boost::str(boost::format("%s as false") % expr);
      std::string got = boost::str(boost::format("%s as true") % expr);

      test_result->on_failure(TestResultItem::create_failure("", expected, got, file, line));
   }
   catch (...)
   {
      auto item = TestResultItem::create_failure("Unhandled exception", file, line);
      test_result->on_failure(std::move(item));
   }
   return false;
}

//---------------------------------------------------------------------------------------

template <typename T>
bool evaluate_close(const T& expected, const T& actual, const T& tolerance, TestResult* test_result, const std::string& file, int line)
{
   try
   {
      if ((actual >= (expected - tolerance)) and (actual <= (expected + tolerance)))
      {
         test_result->on_success(TestResultItem::create_success("", file, line));
         return true;
      }
      

      std::string exp = boost::str(boost::format("Expected %1 +/- %2 but was %3") 
                              % expected 
                              % actual 
                              % tolerance);

      test_result->on_failure(TestResultItem::create_failure("", exp, "", file, line));
   }
   catch (...)
   {
      auto item = TestResultItem::create_failure("Unhandled exception", file, line);
      test_result->on_failure(std::move(item));
   }
   return false;
}

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_EVALUATORS_H */
