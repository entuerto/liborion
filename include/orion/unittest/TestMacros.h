//  TestMacros.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#ifndef ORION_UNITTEST_TESTMACROS_H
#define ORION_UNITTEST_TESTMACROS_H

#define TestSuite(Name)                                      \
   \
namespace Suite##Name                                        \
   \
{                                                      \
      inline Suite& current_test_suite()                     \
      {                                                      \
         static RegisterTestSuiteHelper suite_##Name(#Name); \
         return suite_##Name.suite;                          \
      }                                                      \
   \
}                                                      \
   \
\
namespace Suite##Name

#define RegisterTestCase(SuiteName, TestFunc, ...) \
   \
orion::unittest::RegisterTestHelper _##TestFunc(   \
      current_test_suite(), #TestFunc, TestFunc, ##__VA_ARGS__)

#endif /* ORION_UNITTEST_TESTMACROS_H */
