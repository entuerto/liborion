//
// TestStdOutput.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTSTDOUTPUT_H
#define ORION_UNITTEST_TESTSTDOUTPUT_H

#include <orion/Orion-Stddefs.h>
#include <orion/unittest/TestOutput.h>
#include <orion/unittest/TestUtils.h>

#include <map>
#include <ostream>

namespace orion
{
namespace unittest
{
class Test;

///
/// Outputs the test report on the standard stream (stdout).
///
class API_EXPORT StdOutput : public Output
{
public:
   NO_COPY(StdOutput)
   NO_MOVE(StdOutput)

   StdOutput(std::ostream& stream, ReportLevel report_level = ReportLevel::Error);
   virtual ~StdOutput();

   /// Global test report header
   void write_header(uint64_t test_count) override;

   /// New test suite section
   void suite_start(const TestSuite& suite) override;

   /// Test results information
   void write(const TestResult& test_result) override;

   /// End of test suite section
   void suite_end(const TestSuite& suite) override;

   /// Footer of the output report
   void write_footer(const Totals& totals) override;

private:
   void write_sections();

   std::string fit_text(const std::string& text, int len);

private:
   std::ostream& _stream;
   ReportLevel _report_level;

   std::map<std::string, TestSuiteStats> _test_suite_stats;
   std::multimap<std::string, TestStats> _test_stats;
};

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TESTSTDOUTPUT_H */
