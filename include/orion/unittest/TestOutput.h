// TestOutput.h
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
#ifndef ORION_UNITTEST_TESTOUTPUT_H
#define ORION_UNITTEST_TESTOUTPUT_H

#include <chrono>
#include <istream>

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
#include <orion/unittest/TestResult.h>

namespace orion
{
namespace unittest
{
enum class ReportLevel 
{
   Error    = 0,
   Short    = 1,
   Detailed = 2
};

API_EXPORT std::string to_string(ReportLevel rl);

API_EXPORT std::istream& operator>> (std::istream& in, ReportLevel& report_level);
API_EXPORT std::ostream& operator<< (std::ostream& out, ReportLevel report_level);

struct API_EXPORT TestOutputStats
{
   int count; 
   int passed_count; 
   int failed_count;

   int item_count;
   int passed_item_count; 
   int failed_item_count; 
   
   std::chrono::milliseconds time_elapsed;
};

//!
/*!
 */
class API_EXPORT TestOutput
{
public:
   virtual ~TestOutput() = default;

   virtual void write_header(const std::string& suite_name, int test_count) =0;
   virtual void write(const TestResult* test_result) =0;
   virtual void write_summary(const TestOutputStats& stats) =0;
};

} // namespace orion
} // namespace unittest

#endif /* TESTOUTPUT_H */
