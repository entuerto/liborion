// TestStdOutput.h
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

#ifndef ORION_UNITTEST_TESTSTDOUTPUT_H
#define ORION_UNITTEST_TESTSTDOUTPUT_H

#include <map>
#include <ostream>

#include <orion/Orion-Stddefs.h>
#include <orion/unittest/TestOutput.h>

namespace orion
{
namespace unittest
{
struct API_EXPORT ResultStats
{
   std::string name;
   int count; 
   int passed_item_count; 
   int failed_item_count; 
   std::chrono::milliseconds time_elapsed;
};

//!
/*!
 */
class API_EXPORT TestStdOutput :
   public TestOutput
{
public:
   NO_COPY(TestStdOutput)
   NO_MOVE(TestStdOutput)

   virtual ~TestStdOutput();

   virtual void write_header(const std::string& suite_name, int test_count) override;
   virtual void write(const TestResult* test_result) override;
   virtual void write_summary(const TestOutputStats& stats) override;

   static std::unique_ptr<TestOutput> create(std::ostream& stream, ReportLevel report_level = ReportLevel::Error);

public:
   TestStdOutput(std::ostream& stream, ReportLevel report_level);

private:
   void accumulate_stats(const TestResult* test_result);
 
   void ouput_short();
   void ouput_detailed();

   void write_test_suite(int indent, const ResultStats& r);

   void write_test_case(int indent, const ResultStats& r);

private:
   std::ostream& _stream;
   ReportLevel   _report_level;

   std::map<std::string, ResultStats> _results_by_testsuite;
   std::multimap<std::string, ResultStats> _results;
};

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TESTSTDOUTPUT_H */
