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


#define TEST(TestName, UnitName, ...)  \
   class Test##TestName##UnitName : public orion::unittest::Test  \
   { \
   public: \
      Test##TestName##UnitName() : Test(#TestName "_" #UnitName, UnitTestSuite::suite_name()) \
      {  \
         set_options(this, __VA_ARGS__); \
      }  \
   \
      static Test* create() \
      { \
         return new  Test##TestName##UnitName; \
      } \
   protected: \
        virtual void execute_test_impl(TestResult* test_result) const;  \
   }; \
   \
   orion::unittest::TestAddHelper add_Test##TestName##UnitName(Test::tests(), Test##TestName##UnitName::create()); \
                                                     \
   void Test##TestName##UnitName::execute_test_impl(TestResult* test_result) const

// Macros for testing equalities and inequalities.
//
//  EXPECT_EQ(v1, v2): Tests that v1 == v2
//  EXPECT_NE(v1, v2): Tests that v1 != v2
//  EXPECT_LT(v1, v2): Tests that v1 < v2
//  EXPECT_LE(v1, v2): Tests that v1 <= v2
//  EXPECT_GT(v1, v2): Tests that v1 > v2
//  EXPECT_GE(v1, v2): Tests that v1 >= v2

#define EXPECT_EQ(v1, v2) EXPECT_EVAL(Equals, v1, v2)
#define EXPECT_NE(v1, v2) EXPECT_EVAL(NotEquals, v1, v2)
#define EXPECT_LT(v1, v2) EXPECT_EVAL(Less, v1, v2)
#define EXPECT_LE(v1, v2) EXPECT_EVAL(LessEquals, v1, v2)
#define EXPECT_GT(v1, v2) EXPECT_EVAL(Greater, v1, v2)
#define EXPECT_GE(v1, v2) EXPECT_EVAL(GreaterEquals, v1, v2)

#define EXPECT_ARR_EQ(v1, v2) EXPECT_EVAL(Equals, v1, v2)

#define EXPECT_TRUE(condition) \
{ \
   try \
   { \
      evaluate_true((condition), #condition, test_result, __FILE__, __LINE__); \
   } \
   catch (...) \
   { \
      auto item = TestResultItem::create_failure("Unhandled exception", __FILE__, __LINE__);\
      test_result->on_failure(std::move(item)); \
   } \
}

#define EXPECT_FALSE(condition) \
{ \
   try \
   { \
      evaluate_false((condition), #condition, test_result, __FILE__, __LINE__); \
   } \
   catch (...) \
   { \
      auto item = TestResultItem::create_failure("Unhandled exception", __FILE__, __LINE__);\
      test_result->on_failure(std::move(item)); \
   } \
}

#define EXPECT_EVAL(P, v1, v2) \
{ \
   try \
   { \
      evaluate(P<decltype((v1)), decltype((v2))>{#v1, (v1), #v2, (v2)}, test_result, __FILE__, __LINE__); \
   } \
   catch (...) \
   { \
      auto item = TestResultItem::create_failure("Unhandled exception", __FILE__, __LINE__);\
      test_result->on_failure(std::move(item)); \
   } \
}

#define EXPECT_CLOSE(expected, actual, tolerance) \
{ \
   try \
   { \
      evaluate_close((expected), (actual), (tolerance), test_result, __FILE__, __LINE__); \
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
      evaluate_false((expression), #expression, test_result, __FILE__, __LINE__); \
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
