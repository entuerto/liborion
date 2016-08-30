// TestResultItem.h
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

#ifndef ORION_UNITTEST_TESTRESULTITEM_H
#define ORION_UNITTEST_TESTRESULTITEM_H

#include <memory>
#include <string>
#include <vector>

#include <orion/Orion-Stddefs.h>

namespace orion
{
namespace unittest
{

enum class Result
{
   Failed,
   Passed
};

//!
/*!
 */
class API_EXPORT TestResultItem 
{
public:
   NO_COPY(TestResultItem)
   NO_MOVE(TestResultItem)
   
   virtual ~TestResultItem();

   Result result() const;

   std::string message() const;

   std::string file_name() const;

   int line_number() const;

   static std::unique_ptr<TestResultItem> create_success(const std::string& msg, 
                                                         const std::string& file_name = "", 
                                                               int line_number = 0);

   static std::unique_ptr<TestResultItem> create_failure(const std::string& msg, 
                                                         const std::string& file_name = "", 
                                                               int line_number = 0);

public:
   TestResultItem(Result result, const std::string& msg, const std::string& file_name = "", int line_number = 0);

private:
   Result _result;
   std::string _msg;
   std::string _file_name;
   int _line_number;
};

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TESTRESULTITEM_H */
