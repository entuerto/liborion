// TestResultItem.cpp
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

#include <orion/unittest/TestResultItem.h>

namespace orion
{
namespace unittest
{

TestResultItem::TestResultItem(Result result,const std::string& msg, const std::string& file_name, int line_number) :
   _result(result),
   _msg(msg),
   _file_name(file_name),
   _line_number(line_number)
{
}

TestResultItem::~TestResultItem()
{
}

Result TestResultItem::result() const
{
   return _result;
}

std::string TestResultItem::message() const
{
   return _msg;
}

std::string TestResultItem::file_name() const
{
   return _file_name;
}

int TestResultItem::line_number() const
{
   return _line_number;
}

std::unique_ptr<TestResultItem> TestResultItem::create_success(const std::string& msg, 
                                                               const std::string& file_name, 
                                                                     int line_number)
{
   return std::make_unique<TestResultItem>(Result::Passed, msg, file_name, line_number);
}

std::unique_ptr<TestResultItem> TestResultItem::create_failure(const std::string& msg, 
                                                               const std::string& file_name, 
                                                                     int line_number)
{
   return std::make_unique<TestResultItem>(Result::Failed, msg, file_name, line_number);
}

} // namespace orion
} // namespace unittest
