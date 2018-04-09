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
//#ifdef __COUNTER__ // not standard and may be missing for some compilers
//#define ORION_ANONYMOUS(x) ORION_CAT(x, __COUNTER__)
//#else // __COUNTER__
#define ORION_ANONYMOUS(x) ORION_CAT(x, __LINE__)
//#endif // __COUNTER__


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

#define RegisterTestCase(SuiteName, TestFunc, ...)                \
                                                                  \
orion::unittest::RegisterTestHelper _AutoReg_##TestFunc(          \
      _current_test_suite(), #TestFunc, TestFunc, ##__VA_ARGS__)

#define _RegisterTestCase(TestFunc, ...)                \
                                                                  \
orion::unittest::RegisterTestHelper _AutoReg_##TestFunc(          \
      _current_test_suite(), #TestFunc, TestFunc, ##__VA_ARGS__)

#define _TestCaseImpl(func, ...)         \
   static void func(Test& t);            \
   _RegisterTestCase(func, ##__VA_ARGS__); \
   static void func(Test& t)


#define TestCase(label, ...) \
   _TestCaseImpl(ORION_ANONYMOUS(_test_case_), orion::unittest::option::Label{label}, ##__VA_ARGS__)


//-------------------------------------------------------------------------------------------------

//#define check_throws    
//#define check_throws_as 
//#define check_nothrow   

#define check_eq(ex, ac, ...) t.xassert<eq>((ex), (ac), _src_loc, ##__VA_ARGS__);
#define check_ne(ex, ac, ...) t.xassert<ne>((ex), (ac), _src_loc, ##__VA_ARGS__);
#define check_ge(ex, ac, ...) t.xassert<ge>((ex), (ac), _src_loc, ##__VA_ARGS__);
#define check_gt(ex, ac, ...) t.xassert<gt>((ex), (ac), _src_loc, ##__VA_ARGS__);
#define check_le(ex, ac, ...) t.xassert<le>((ex), (ac), _src_loc, ##__VA_ARGS__);
#define check_lt(ex, ac, ...) t.xassert<lt>((ex), (ac), _src_loc, ##__VA_ARGS__);
       
#define check_true(exp, ...) t.xassert<true>((exp), _src_loc, ##__VA_ARGS__);
#define check_false(exp, ...) t.xassert<false>((exp), _src_loc, ##__VA_ARGS__);

//#define check           
//#define check_not

#endif /* ORION_UNITTEST_TESTMACROS_H */
