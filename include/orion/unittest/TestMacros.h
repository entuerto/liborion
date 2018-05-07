//
// TestMacros.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTMACROS_H
#define ORION_UNITTEST_TESTMACROS_H

//-------------------------------------------------------------------------------------------------

// Stringify an identifier
#define ORION_TOSTRING_IMPL(...) #__VA_ARGS__
#define ORION_TOSTRING(...) ORION_TOSTRING_IMPL(__VA_ARGS__)

// Macros for string concatenation
#define ORION_CAT_IMPL(s1, s2) s1##s2
#define ORION_CAT(s1, s2) ORION_CAT_IMPL(s1, s2)

// Macros for anonymous variable name generation
#ifdef __COUNTER__ // not standard and may be missing for some compilers
#define ORION_ANONYMOUS(x) ORION_CAT(x, __COUNTER__)
#else // __COUNTER__
#define ORION_ANONYMOUS(x) ORION_CAT(x, __LINE__)
#endif // __COUNTER__

// clang-format off

//-------------------------------------------------------------------------------------------------

#define Section(Name, ...)                                               \
                                                                         \
namespace Suite##Name                                                    \
{                                                                        \
   inline TestSuite& _current_test_suite()                               \
   {                                                                     \
      static RegisterTestSuiteHelper suite_##Name(#Name, ##__VA_ARGS__); \
      return suite_##Name.suite;                                         \
   }                                                                     \
}                                                                        \
                                                                         \
namespace Suite##Name

//-------------------------------------------------------------------------------------------------

#define RegisterTestCase(TestFunc, ...)                                \
unittest::RegisterTestHelper _AutoReg_##TestFunc(_current_test_suite(), \
                                                 #TestFunc, TestFunc, ##__VA_ARGS__)

#define _TestCaseImpl(func, ...)           \
   static void func(Test& t);              \
   RegisterTestCase(func, ##__VA_ARGS__); \
   static void func(Test& t)


#define TestCase(label, ...) \
   _TestCaseImpl(ORION_ANONYMOUS(_test_case_), unittest::option::Label{label}, ##__VA_ARGS__)


//-------------------------------------------------------------------------------------------------

/// Expects that an exception (of any type) is thrown during evaluation of the expression.
#define check_throws(expr, ...)                                                             \
   do                                                                                       \
   {                                                                                        \
      try                                                                                   \
      {                                                                                     \
         expr;                                                                              \
         t.expected_exception_not_thrown(#expr, _src_loc, ##__VA_ARGS__);                   \
      }                                                                                     \
      catch(...)                                                                            \
      {                                                                                     \
         t.exception_thrown_as_expected(std::current_exception(), _src_loc, ##__VA_ARGS__); \
      }                                                                                     \
   } while((void)0, 0);
  
/// Expects that an exception of the _specified type_ is thrown during evaluation of the expression.
#define check_throws_as(expr, exception_type, ...)                                          \
   do                                                                                       \
   {                                                                                        \
      try                                                                                   \
      {                                                                                     \
         expr;                                                                              \
         t.expected_exception_not_thrown(#expr, _src_loc, ##__VA_ARGS__);                   \
      }                                                                                     \
      catch(const exception_type& e)                                                        \
      {                                                                                     \
         t.exception_thrown_as_expected(e, _src_loc, ##__VA_ARGS__);                        \
      }                                                                                     \
      catch(...)                                                                            \
      {                                                                                     \
         t.unexpected_exception_thrown(std::current_exception(), _src_loc, ##__VA_ARGS__);  \
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
         t.unexpected_exception_thrown(std::current_exception(), _src_loc, ##__VA_ARGS__);  \
      }                                                                                     \
   } while((void)0, 0);   

#define check_eq(ex, ac, ...) t.xassert<eq>((ex), (ac), _src_loc, ##__VA_ARGS__);
#define check_ne(ex, ac, ...) t.xassert<ne>((ex), (ac), _src_loc, ##__VA_ARGS__);
#define check_ge(ex, ac, ...) t.xassert<ge>((ex), (ac), _src_loc, ##__VA_ARGS__);
#define check_gt(ex, ac, ...) t.xassert<gt>((ex), (ac), _src_loc, ##__VA_ARGS__);
#define check_le(ex, ac, ...) t.xassert<le>((ex), (ac), _src_loc, ##__VA_ARGS__);
#define check_lt(ex, ac, ...) t.xassert<lt>((ex), (ac), _src_loc, ##__VA_ARGS__);
       
#define check_true(exp, ...) t.xassert<true>((exp), _src_loc, ##__VA_ARGS__);
#define check_false(exp, ...) t.xassert<false>((exp), _src_loc, ##__VA_ARGS__);

//#define check           

// clang-format on

#endif /* ORION_UNITTEST_TESTMACROS_H */
