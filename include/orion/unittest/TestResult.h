// TestResult.h
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

#ifndef ORION_UNITTEST_TESTRESULT_H
#define ORION_UNITTEST_TESTRESULT_H

#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
#include <orion/Timer.h>
#include <orion/unittest/TestResultItem.h>

namespace orion
{
namespace unittest
{
//!
/*!
 */
class API_EXPORT TestResult 
{
public:
   DECLARE_POINTERS(TestResult)

   ~TestResult();

   std::string name() const;
   std::string suite_name() const;

   bool passed() const;
   bool failed() const;

   int failed_item_count() const;
   int passed_item_count() const;

   double time_elapsed() const;

   const TestResultItem::SharedPtrVector& result_items() const;

   void on_start();
   void on_failure(TestResultItem::SharedPtr item);
   void on_success(TestResultItem::SharedPtr item);
   void on_end();

   static TestResult::SharedPtr create(const std::string& name, const std::string& suite_name = "DefaultSuite");

protected:
   TestResult(const std::string& name, const std::string& suite_name);

private:
   std::string _name;
   std::string _suite_name;
   int _failed_item_count;
   int _passed_item_count;
   Timer _timer;
   TestResultItem::SharedPtrVector _result_items;
};

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TESTRESULT_H */
