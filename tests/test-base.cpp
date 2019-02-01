//
//  test-base.cpp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Test.h>

#include <orion/Assert.h>
#include <orion/Utils.h>

#include <limits> 
#include <vector> 

using namespace orion;
using namespace orion::unittest;
using namespace orion::unittest::option;

Section(OrionCore_Base, Label{"Base"})
{

int expect(int i)
{
   Expects(i > 0 and i < 10);
   return i;
}

int ensure(int i)
{
   Ensures(i > 0 and i < 10);
   return i;
}

void unreachable()
{
   AssertUnreachable("Test unreachable");
}

void f(int& i) 
{ 
   i += 1; 
}

//--------------------------------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------------------------------
TestCase("Expects(): Allows a true expression")
{
   check_true(expect(2) == 2);
}

TestCase("Ensures(): Allows a true expression")
{
   check_true(ensure(2) == 2);
}

TestCase("Expects(): Terminates on a false expression")
{
   check_throws(expect(100));
}

TestCase("Ensures(): Terminates on a false expression")
{
   check_throws(ensure(100));
}

TestCase("AssertUnreachable(): Terminates on assertation")
{
   check_throws(unreachable());
}

TestCase("FinalAction: function lambda")
{
   int i = 0;
   {
      auto _ = finally([&]() { f(i); });
      check_eq(i, 0);
   }
   check_eq(i, 1);
}

TestCase("FinalAction: function lambda with move")
{
   int i = 0;
   {
      auto _1 = finally([&]() { f(i); });
      {
         auto _2 = std::move(_1);
         check_eq(i, 0);
      }
      check_eq(i, 1);
      {
         auto _2 = std::move(_1);
         check_eq(i, 1);
      }
      check_eq(i, 1);
   }
   check_eq(i, 1);
}

TestCase("narrow_cast")
{
    int n = 120;
    char c = narrow_cast<char>(n);
    check_true(c == 120);

    n = 300;
    unsigned char uc = narrow_cast<unsigned char>(n);
    check_true(uc == 44);
}

TestCase("narrow")
{
    int n = 120;
    const char c = narrow<char>(n);
    check_true(c == 120);

    n = 300;
    check_throws_as(narrow<char>(n), NarrowingError);

    const auto int32_max = std::numeric_limits<int32_t>::max();
    const auto int32_min = std::numeric_limits<int32_t>::min();

    check_true(narrow<uint32_t>(int32_t(0)) == 0);
    check_true(narrow<uint32_t>(int32_t(1)) == 1);
    check_true(narrow<uint32_t>(int32_max) == static_cast<uint32_t>(int32_max));

    check_throws_as(narrow<uint32_t>(int32_t(-1)), NarrowingError);
    check_throws_as(narrow<uint32_t>(int32_min), NarrowingError);

    n = -42;
    check_throws_as(narrow<unsigned>(n), NarrowingError);
}

TestCase("at() - existing C-array element")
{
   int a[] = {1, 2, 3, 4};

   check_eq(at(a, 0), 1);
   check_eq(at(a, 1), 2);
   check_eq(at(a, 2), 3);
   check_eq(at(a, 3), 4);
}

TestCase("at() - non-existing C-array element")
{
   int a[] = {1, 2, 3, 4};

   check_throws(at(a, 4));
}

TestCase("at() - existing std::array element")
{
   std::array<int, 4> a = {1, 2, 3, 4};

   check_eq(at(a, 0), 1);
   check_eq(at(a, 1), 2);
   check_eq(at(a, 2), 3);
   check_eq(at(a, 3), 4);
}

TestCase("at() - non-existing std::array element")
{
   std::array<int, 4> a = {1, 2, 3, 4};

   check_throws(at(a, 4));
}

TestCase("at() - existing std::vector element")
{
   std::vector<int> v = {1, 2, 3, 4};

   check_eq(at(v, 0), 1);
   check_eq(at(v, 1), 2);
   check_eq(at(v, 2), 3);
   check_eq(at(v, 3), 4);
}

TestCase("at() - non-existing std::vector element")
{
   std::vector<int> v = {1, 2, 3, 4};

   check_throws(at(v, 4));
}

TestCase("at() - existing std::initializer_list element")
{
   std::initializer_list<int> l = {1, 2, 3, 4};

   check_eq(at(l, 0), 1);
   check_eq(at(l, 1), 2);
   check_eq(at(l, 2), 3);
   check_eq(at(l, 3), 4);
}

TestCase("at() - non-existing std::initializer_list element")
{
   std::initializer_list<int> l = {1, 2, 3, 4};

   check_throws(at(l, 4));
}

} // Section
