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

#include <orion/MemoryUtils.h>
#include <orion/unittest/TestResult.h>

namespace orion
{
namespace unittest
{
//!
/*!
 */
class TestOutput
{
public:
   DECLARE_POINTERS(TestOutput)

   virtual ~TestOutput() {}

   virtual void write(const TestResult::SharedPtr& test_result) =0;
   virtual void write_summary(int failure_count, int failed_item_count, int test_count, double time_elapsed) =0;
};

} // namespace orion
} // namespace unittest

#endif /* TESTOUTPUT_H */
