// TestStdOutput.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#ifndef ORION_UNITTEST_TESTSTDOUTPUT_H
#define ORION_UNITTEST_TESTSTDOUTPUT_H

#include <orion/Orion-Stddefs.h>
#include <orion/unittest/TestOutput.h>

#include <map>
#include <ostream>

namespace orion
{
namespace unittest
{
class Test;
///
///
///
class API_EXPORT StdOutput : public Output
{
public:
   NO_COPY(StdOutput)
   NO_MOVE(StdOutput)

   StdOutput(std::ostream& stream, ReportLevel report_level = ReportLevel::Error);
   virtual ~StdOutput();

   /// Global test report header
   void write_header(int test_count) override;

   /// Test suite header
   void write_suite_header(const Suite& suite) override;

   /// Test results information
   void write(const TestResult& test_result) override;

   /// Summary of the test suite
   void write_suite_summary(const Suite& suite) override;

   /// Summary of all tests
   void write_summary(const OutputStats& stats) override;

private:
   void write_test_case(int indent, const Test& test);

private:
   std::ostream& _stream;
   ReportLevel _report_level;
   int _indent;
};

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TESTSTDOUTPUT_H */
