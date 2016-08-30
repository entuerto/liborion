// TestMacros.h
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

#ifndef ORION_UNITTEST_TESTMACROS_H
#define ORION_UNITTEST_TESTMACROS_H

#define TEST_SUITE(Name) \
   namespace Suite##Name \
   { \
      namespace UnitTestSuite \
      { \
      inline const char* suite_name() { return #Name; } \
      } \
   } \
   namespace Suite##Name


#define TEST(Name)  \
   class Test##Name : public orion::unittest::Test  \
   { \
   public: \
      Test##Name() : Test(#Name, UnitTestSuite::suite_name()) {}  \
   \
      static Test* create() \
      { \
         return new  Test##Name; \
      } \
   protected: \
        virtual void execute_test_impl(TestResult* test_result) const;  \
   }; \
   \
   orion::unittest::TestAddHelper add_##Name(Test::tests(), Test##Name::create()); \
                                                     \
   void Test##Name::execute_test_impl(TestResult* test_result) const


#define EXPECT(expression) \
{ \
   try \
   { \
      evaluate(equals_true((expression)), test_result, \
         TestResultItem::create_success("Expected " #expression, __FILE__, __LINE__), \
         TestResultItem::create_failure("Expected " #expression, __FILE__, __LINE__)); \
   } \
   catch (...) \
   { \
      auto item = TestResultItem::create_failure("Unhandled exception in EXPECT(" #expression ")", __FILE__, __LINE__);\
      test_result->on_failure(std::move(item)); \
   } \
}


#define EXPECT_CLOSE(expected, actual, tolerance) \
{ \
   try \
   { \
      evaluate(evaluate_close((expected), (actual), (tolerance)), test_result, \
         TestResultItem::create_success("Expected " #expected " +/- " #tolerance " but was " #actual, __FILE__, __LINE__), \
         TestResultItem::create_failure("Expected " #expected " +/- " #tolerance " but was " #actual, __FILE__, __LINE__)); \
   } \
   catch (...) \
   { \
      auto item = TestResultItem::create_failure("Unhandled exception in EXPECT_CLOSE", __FILE__, __LINE__);\
      test_result->on_failure(std::move(item)); \
   } \
}


#define EXPECT_THROW(expression, ExpectedException) \
{ \
   bool caught = false; \
   try { (expression); } \
   catch (const ExpectedException&) { caught = true; } \
   catch (...) {} \
   if (caught) \
   { \
      auto item = TestResultItem::create_success("Expected exception " #ExpectedException, __FILE__, __LINE__);\
      test_result->on_success(std::move(item)); \
   } \
   else \
   { \
      auto item = TestResultItem::create_failure("Expected exception " #ExpectedException, __FILE__, __LINE__);\
      test_result->on_failure(std::move(item)); \
   } \
}


#define FAIL_IF(expression) \
{ \
   try \
   { \
      evaluate(equals_false((expression)), test_result, \
         TestResultItem::create_success(#expression, __FILE__, __LINE__), \
         TestResultItem::create_failure(#expression, __FILE__, __LINE__)); \
   } \
   catch (...) \
   { \
      auto item = TestResultItem::create_failure("Unhandled exception in FAIL_IF(" #expression ")", __FILE__, __LINE__);\
      test_result->on_failure(std::move(item)); \
   } \
}


#define FAIL_THROW(expression, UnexpectedException) \
{ \
   bool caught = false; \
   try { (expression); } \
   catch (const UnexpectedException&) { caught = true; } \
   catch (...) {} \
   if (caught) \
   { \
      auto item = TestResultItem::create_failure("Unexpected exception " #UnexpectedException, __FILE__, __LINE__);\
      test_result->on_failure(std::move(item)); \
   } \
   else \
   { \
      auto item = TestResultItem::create_success("Unexpected exception " #UnexpectedException, __FILE__, __LINE__);\
      test_result->on_success(std::move(item)); \
   } \
}

#endif /* ORION_UNITTEST_TESTMACROS_H */
