//  TestMacros.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#ifndef ORION_UNITTEST_TESTMACROS_H
#define ORION_UNITTEST_TESTMACROS_H

#define TestSuite(Name, ...)                                             \
                                                                         \
namespace Suite##Name                                                    \
{                                                                        \
   inline Suite& _test_suite_##Name()                                    \
   {                                                                     \
      static RegisterTestSuiteHelper suite_##Name(#Name, ##__VA_ARGS__); \
      return suite_##Name.suite;                                         \
   }                                                                     \
}                                                                        \
                                                                         \
namespace Suite##Name

#define RegisterTestCase(SuiteName, TestFunc, ...)                \
                                                                  \
orion::unittest::RegisterTestHelper _AutoReg_##TestFunc(          \
      _test_suite_##SuiteName(), #TestFunc, TestFunc, ##__VA_ARGS__)

#endif /* ORION_UNITTEST_TESTMACROS_H */
