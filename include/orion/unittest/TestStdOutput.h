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

#include <ostream>

#include <orion/Orion-Stddefs.h>
#include <orion/unittest/TestOutput.h>

namespace orion
{
namespace unittest
{
//!
/*!
 */
class API_EXPORT TestStdOutput :
   public TestOutput
{
public:
   virtual ~TestStdOutput();

   virtual void write(const TestResult::SharedPtr& test_result);
   virtual void write_summary(int failure_count, int failed_item_count, int test_count, double time_elapsed);

   static TestOutput::SharedPtr create(std::ostream& stream);

protected:
   TestStdOutput(std::ostream& stream);

private:
   std::ostream& _stream;
};

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TESTSTDOUTPUT_H */
