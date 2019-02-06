//
//  test-base.cpp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Test.h>

#include <orion/Common.h>
#include <orion/Log.h>
#include <orion/Utils.h>

#include <limits> 
#include <vector> 

using namespace orion;
using namespace orion::unittest;

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
    auto uc = narrow_cast<unsigned char>(n);
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

TestCase("Span<>: Default construction")
{
   Span<int> s;
   check_true((s.empty() and s.data() == nullptr));

   Span<const int> cs;
   check_true((cs.empty() and cs.data() == nullptr));
}

TestCase("Span<>: Construct from a nullptr and a zero size")
{
   using IndexType = Span<int>::index_type;

   Span<int> v{nullptr, IndexType{0}};
   Span<const int> w{nullptr, IndexType{0}};

   check_eq(v.size(), 0);
   check_eq(w.size(), 0);

   auto macro = []() { const Span<int> s{nullptr, 1}; };

   check_throws(macro());

   auto const_macro = []() { const Span<const int> cs{nullptr, 1}; };

   check_throws(const_macro());
}

TestCase("Span<>: Construct from a C-array")
{
   int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, };

   Span<int> v{arr};
   Span<const int> w{arr};

   check_true(std::equal(v.begin(), v.end(), arr));
   check_true(std::equal(w.begin(), w.end(), arr));
}

TestCase("Span<>: Construct from a C-array")
{
   const int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, };

   Span<const int> v{arr};

   check_true(std::equal(v.begin(), v.end(), arr));
}

TestCase("Span<>: Construct from a C-array with size via decay to pointer")
{
   int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, };

   Span<int> v{arr, array_size(arr)};
   Span<const int> w{arr, array_size(arr)};

   check_true(std::equal(v.begin(), v.end(), arr));
   check_true(std::equal(w.begin(), w.end(), arr));
}

TestCase("Span<>: Construct from a std::array<>")
{
   std::array<int, 9> arr = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, }};

   Span<int> v{arr};

   check_true(std::equal(v.begin(), v.end(), arr.begin()));

   std::array<const int, 9> carr = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, }};

   Span<const int> cv{arr};

   check_true(std::equal(cv.begin(), cv.end(), carr.begin()));
}

TestCase("Span<>: Construct from a container (std::vector<>)")
{
   std::vector<int> vec = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, }};

   Span<int> v{vec};
   Span<const int> w{vec};

   check_true(std::equal(v.begin(), v.end(), vec.begin()));
   check_true(std::equal(w.begin(), w.end(), vec.begin()));
}

} // Section
