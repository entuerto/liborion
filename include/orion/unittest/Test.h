//
// Test.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TEST_H
#define ORION_UNITTEST_TEST_H

#include <orion/Config.h>

#include <orion/unittest/TestBase.h>
#include <orion/unittest/TestDecomposer.h>
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

//-------------------------------------------------------------------------------------------------

template<typename LhsT>
using UnaryPredFunc = std::function<bool(const LhsT& lhs)>;

template<typename LhsT, typename RhsT>
using BinPredFunc = std::function<bool(const LhsT& lhs, const RhsT& rhs)>;

template<typename LhsT>
class UnaryPredicate;

template<typename LhsT, typename RhsT>
class BinaryPredicate;

//-------------------------------------------------------------------------------------------------

///
///
///
class Test : public TestInfo
{
public:
   explicit Test(std::string name);
   Test(std::string name, TestCaseFunc&& f);
   ~Test() override = default;

   constexpr const TestResult& test_result() const;

   constexpr TestCaseFunc& case_func();

   const TestResult& invoke() const;

   template<typename LhsT, typename RhsT, typename... Args>
   void asserter(BinaryExpression<LhsT, RhsT> expr, const std::string& expr_text, Args... args);

   template<typename... Args>
   void asserter(ExpressionLhs<bool> expr, const std::string& expr_text, Args... args);

   template<typename T, typename... Args>
   void asserter(UnaryPredicate<T> pred, Args... args);

   template<typename T1, typename T2, typename... Args>
   void asserter(BinaryPredicate<T1, T2> pred, Args... args);

   template<typename... Args>
   void fail(Args... args);

   template<typename... Args>
   void fail_if(bool value, Args... args);

   //
   // Exception handling
   // 

   template<typename... Args>
   void expected_exception_not_thrown(std::string expr, Args... args);

   template<typename ExceptionT, typename... Args>
   void exception_thrown_as_expected(const ExceptionT& e, Args... args);

   template<typename... Args>
   void exception_thrown_as_expected(const std::exception_ptr& eptr, Args... args);

   template<typename ExceptionT, typename... Args>
   void unexpected_exception_thrown(const ExceptionT& e, Args... args);

   template<typename... Args>
   void unexpected_exception_thrown(const std::exception_ptr& eptr, Args... args);

protected:
   virtual void do_invoke() const;

private:
   mutable TestResult _test_result;

   TestCaseFunc _func;
};

} // namespace unittest
} // namespace orion

#include <orion/unittest/impl/Test.ipp>

#endif /* ORION_UNITTEST_TEST_H */
