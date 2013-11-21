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

#include <orion/unittest/TestResult.h>

namespace orion
{
namespace unittest
{

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

template <typename P>
bool evaluate(P predicate, TestResult::SharedPtr& test_result, TestResultItem::SharedPtr success, TestResultItem::SharedPtr failure)
{
   try
   {
      if (predicate)
      {
         test_result->on_success(success);
         return true;
      }
      test_result->on_failure(failure);
   }
   catch (...)
   {
      test_result->on_failure(TestResultItem::create_failure("Unhandled exception", __FILE__, __LINE__));
   }
   return false;
}

template <typename T>
bool evaluate_close(const T& expected, const T& actual, const T& tolerance)
{
   return (actual >= (expected - tolerance)) and (actual <= (expected + tolerance));
}

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_EVALUATORS_H */
