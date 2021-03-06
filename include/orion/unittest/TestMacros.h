//
// TestMacros.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTMACROS_H
#define ORION_UNITTEST_TESTMACROS_H

#include <orion/Macros.h>

// clang-format off

//-------------------------------------------------------------------------------------------------

#define Section(Name, ...)                                                  \
                                                                            \
namespace Suite##Name                                                       \
{                                                                           \
   inline TestSuite& _current_test_suite()                                  \
   {                                                                        \
      try                                                                   \
      {                                                                     \
         static RegisterTestSuiteHelper suite_##Name(#Name, ##__VA_ARGS__); \
         return suite_##Name.suite;                                         \
      }                                                                     \
      catch (const std::exception& e)                                       \
      {                                                                     \
         log::fatal("An unexpected, exception was thrown: ", e, DbgSrcLoc); \
      }                                                                     \
   }                                                                        \
}                                                                           \
                                                                            \
namespace Suite##Name

//-------------------------------------------------------------------------------------------------

#define RegisterTestCase(TestName, ...)  \
const TestName AutoReg_##TestName = make_test<TestName>(_current_test_suite(), ##__VA_ARGS__)

//-------------------------------------------------------------------------------------------------

#define _TestCaseImpl(TestName, ...)                          \
class TestName : public unittest::Test                        \
{                                                             \
public:                                                       \
   DEFAULT_COPY(TestName)                                     \
   DEFAULT_MOVE(TestName)                                     \
                                                              \
   TestName()                                                 \
      : Test(ORION_TOSTRING(TestName))                        \
   {                                                          \
   }                                                          \
   ~TestName() override = default;                            \
                                                              \
protected:                                                    \
   void do_invoke() const override;                           \
};                                                            \
                                                              \
RegisterTestCase(TestName, ##__VA_ARGS__);  \
                                                              \
void TestName::do_invoke() const


#define TestCase(label, ...) \
   _TestCaseImpl(ORION_ANONYMOUS(_test_case_), unittest::Label{label}, ##__VA_ARGS__)


//-------------------------------------------------------------------------------------------------

/// Expects that an exception (of any type) is thrown during evaluation of the expression.
#define check_throws(expr, ...)                                                             \
   do                                                                                       \
   {                                                                                        \
      try                                                                                   \
      {                                                                                     \
         expr;                                                                              \
         expected_exception_not_thrown(ORION_TOSTRING(expr), DbgSrcLoc, ##__VA_ARGS__);     \
      }                                                                                     \
      catch(...)                                                                            \
      {                                                                                     \
         exception_thrown_as_expected(std::current_exception(), DbgSrcLoc, ##__VA_ARGS__);  \
      }                                                                                     \
   } while((void)0, 0);
  
/// Expects that an exception of the _specified type_ is thrown during evaluation of the expression.
#define check_throws_as(expr, exception_type, ...)                                          \
   do                                                                                       \
   {                                                                                        \
      try                                                                                   \
      {                                                                                     \
         expr;                                                                              \
         expected_exception_not_thrown(ORION_TOSTRING(expr), DbgSrcLoc, ##__VA_ARGS__);     \
      }                                                                                     \
      catch(const exception_type& e)                                                        \
      {                                                                                     \
         exception_thrown_as_expected(e, DbgSrcLoc, ##__VA_ARGS__);                         \
      }                                                                                     \
      catch(...)                                                                            \
      {                                                                                     \
         unexpected_exception_thrown(std::current_exception(), DbgSrcLoc, ##__VA_ARGS__);   \
      }                                                                                     \
   } while((void)0, 0);

/// Expects that no exception is thrown during evaluation of the expression.
#define check_no_throw(expr, ...)                                                           \
   do                                                                                       \
   {                                                                                        \
      try                                                                                   \
      {                                                                                     \
         expr;                                                                              \
      }                                                                                     \
      catch(...)                                                                            \
      {                                                                                     \
         unexpected_exception_thrown(std::current_exception(), DbgSrcLoc, ##__VA_ARGS__);   \
      }                                                                                     \
   } while((void)0, 0);   

#define check(...)                                                                          \
   do                                                                                       \
   {                                                                                        \
      ORION_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS                                          \
      asserter(ExpressionDecomposer() << __VA_ARGS__, ORION_TOSTRING(__VA_ARGS__), DbgSrcLoc); \
      ORION_INTERNAL_UNSUPPRESS_PARENTHESES_WARNINGS                                        \
   } while((void)0, 0);   

#define check_eq(ex, ac, ...)                                                               \
   do                                                                                       \
   {                                                                                        \
      ORION_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS                                          \
      asserter(eval_eq(ex, ac, ORION_TOSTRING(ex == ac)), DbgSrcLoc, ##__VA_ARGS__);        \
      ORION_INTERNAL_UNSUPPRESS_PARENTHESES_WARNINGS                                        \
   } while((void)0, 0);
   

#define check_ne(ex, ac, ...)                                                               \
   do                                                                                       \
   {                                                                                        \
      ORION_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS                                          \
      asserter(eval_ne(ex, ac, ORION_TOSTRING(ex != ac)), DbgSrcLoc, ##__VA_ARGS__);        \
      ORION_INTERNAL_UNSUPPRESS_PARENTHESES_WARNINGS                                        \
   } while((void)0, 0);

#define check_ge(ex, ac, ...)                                                               \
   do                                                                                       \
   {                                                                                        \
      ORION_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS                                          \
      asserter(eval_ge(ex, ac, ORION_TOSTRING(ex >= ac)), DbgSrcLoc, ##__VA_ARGS__);        \
      ORION_INTERNAL_UNSUPPRESS_PARENTHESES_WARNINGS                                        \
   } while((void)0, 0);

#define check_gt(ex, ac, ...)                                                               \
   do                                                                                       \
   {                                                                                        \
      ORION_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS                                          \
      asserter(eval_gt(ex, ac, ORION_TOSTRING(ex > ac)), DbgSrcLoc, ##__VA_ARGS__);         \
      ORION_INTERNAL_UNSUPPRESS_PARENTHESES_WARNINGS                                        \
   } while((void)0, 0);

#define check_le(ex, ac, ...)                                                               \
   do                                                                                       \
   {                                                                                        \
      ORION_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS                                          \
      asserter(eval_le(ex, ac, ORION_TOSTRING(ex <= ac)), DbgSrcLoc, ##__VA_ARGS__);        \
      ORION_INTERNAL_UNSUPPRESS_PARENTHESES_WARNINGS                                        \
   } while((void)0, 0);

#define check_lt(ex, ac, ...)                                                               \
   do                                                                                       \
   {                                                                                        \
      ORION_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS                                          \
      asserter(eval_lt(ex, ac, ORION_TOSTRING(ex < ac)), DbgSrcLoc, ##__VA_ARGS__);         \
      ORION_INTERNAL_UNSUPPRESS_PARENTHESES_WARNINGS                                        \
   } while((void)0, 0);
       
#define check_true(exp, ...)                                                                \
   do                                                                                       \
   {                                                                                        \
      ORION_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS                                          \
      asserter(eval_true(exp, ORION_TOSTRING(exp)), DbgSrcLoc, ##__VA_ARGS__);              \
      ORION_INTERNAL_UNSUPPRESS_PARENTHESES_WARNINGS                                        \
   } while((void)0, 0);

#define check_false(exp, ...)                                                               \
   do                                                                                       \
   {                                                                                        \
      ORION_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS                                          \
      asserter(eval_false(exp, ORION_TOSTRING(exp)), DbgSrcLoc, ##__VA_ARGS__);             \
      ORION_INTERNAL_UNSUPPRESS_PARENTHESES_WARNINGS                                        \
   } while((void)0, 0);


// clang-format on

#endif /* ORION_UNITTEST_TESTMACROS_H */
