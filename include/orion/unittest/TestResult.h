// TestResult.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#ifndef ORION_UNITTEST_TESTRESULT_H
#define ORION_UNITTEST_TESTRESULT_H

#include <chrono>
#include <string>
#include <vector>

#include <orion/Orion-Stddefs.h>
#include <orion/Utils.h>

namespace orion
{
namespace unittest
{
//---------------------------------------------------------------------------------------

#define _src_loc SourceLocation{__FILE__, __LINE__}

struct SourceLocation
{
   std::string file_name;
   int line_number;
};

//---------------------------------------------------------------------------------------

enum class Result
{
   Passed,
   Failed,
   Exception,
   Skipped
};

//---------------------------------------------------------------------------------------

struct API_EXPORT ResultItem
{
   Result result;

   std::string msg;
   std::string expected;
   std::string actual;

   SourceLocation src_loc;
};

//!
/*!
 */
class API_EXPORT TestResult 
{
public:
   TestResult(const std::string& name, const std::string& suite_name);
   ~TestResult() = default;

   std::string name() const;
   std::string suite_name() const;

   bool passed() const;
   bool failed() const;

   int item_count() const;
   int failed_item_count() const;
   int passed_item_count() const;
   int skipped_item_count() const;

   std::chrono::milliseconds time_elapsed() const;

   const std::vector<ResultItem>& result_items() const;

   void on_start();
   void on_end();


   template<typename... Args>
   void log_success(Args... args);

   template<typename T, typename... Args>
   void log_failure(const T& expected, const T& actual, Args... args);

   template<typename... Args>
   void log_failure(Args... args);

   template<typename E, typename... Args>
   void log_exception(const E& e, Args... args);

   template<typename... Args>
   void log_exception(std::exception_ptr eptr, Args... args);

   template<typename... Args>
   void log_skipped(Args... args);

private:
   std::string _name;
   std::string _suite_name;
   
   int _failed_item_count;
   int _passed_item_count;
   int _skipped_item_count;

   std::chrono::time_point<std::chrono::steady_clock> _time_point_start;
   std::chrono::time_point<std::chrono::steady_clock> _time_point_end;

   std::vector<ResultItem> _result_items;
};

struct StringConcat
{
   std::string& text;

   template<typename T>
   void operator()(T) {  }

   void operator()(const char* v)        { text += v; }
   void operator()(const std::string& v) { text += v; }
};

template<typename... Args>
void TestResult::log_success(Args... args)
{
   ++_passed_item_count;

   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   _result_items.push_back(ResultItem{
      Result::Passed,
      msg,
      "",
      "",
      get_value<SourceLocation>(t, SourceLocation{})
   });
}

template<typename T, typename... Args>
void TestResult::log_failure(const T& expected, const T& actual, Args... args)
{
   ++_failed_item_count;

   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   _result_items.push_back(ResultItem{
      Result::Failed,
      msg,
      get_as_string(expected),
      get_as_string(actual),
      get_value<SourceLocation>(t, SourceLocation{})
   });
}

template<typename... Args>
void TestResult::log_failure(Args... args)
{
   ++_failed_item_count;

   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   _result_items.push_back(ResultItem{
      Result::Failed,
      msg,
      "",
      "",
      get_value<SourceLocation>(t, SourceLocation{})
   });
}

template<typename E, typename... Args>
void TestResult::log_exception(const E& e, Args... args)
{
   ++_failed_item_count;

   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   msg += type_name(e) + " (" + e.what() + ") ";

   _result_items.push_back(ResultItem{
      Result::Exception,
      msg,
      "",
      "",
      get_value<SourceLocation>(t, SourceLocation{})
   });
}

template<typename... Args>
void TestResult::log_exception(std::exception_ptr eptr, Args... args)
{
   ++_failed_item_count;

   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   msg += type_name(eptr);
   
   _result_items.push_back(ResultItem{
      Result::Exception,
      msg,
      "",
      "",
      get_value<SourceLocation>(t, SourceLocation{})
   });
}

template<typename... Args>
void TestResult::log_skipped(Args... args)
{
   ++_skipped_item_count;

   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   _result_items.push_back(ResultItem{
      Result::Skipped,
      msg,
      "",
      "",
      get_value<SourceLocation>(t, SourceLocation{})
   });
}

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TESTRESULT_H */
