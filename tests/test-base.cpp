//
//  test-base.cpp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Test.h>

#include <orion/Assert.h>

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

} // Section
