//
// Test.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TEST_IPP
#define ORION_UNITTEST_TEST_IPP

#include <orion/Utils.h>

#include <string>

namespace orion
{
namespace unittest
{

template<class Func, typename T, typename... Args>
inline void Test::assert(const T& expected, const T& actual, Args... args)
{
   auto t = std::make_tuple(args...);

   if (has_type<option::Disabled>(t))
   {
      set_option(get_value<option::Disabled>(t, option::Disabled{}));

      _test_result.log_skipped(disabled_reason());
      return;
   }

   Func cmp;

   if (cmp(expected, actual))
   {
      _test_result.log_success();
      return;
   }

   _test_result.log_failure(expected, actual, args...);
}

template<int op, typename T, typename... Args>
inline void Test::assert(const T& expected, const T& actual, Args... args)
{
   auto t = std::make_tuple(args...);

   if (has_type<option::Disabled>(t))
   {
      set_option(get_value<option::Disabled>(t, option::Disabled{}));

      _test_result.log_skipped(disabled_reason());
      return;
   }

   bool success = false;

   switch (op)
   {
      case lt:
         success = expected < actual;
         break;
      case le:
         success = expected <= actual;
         break;
      case eq:
         success = expected == actual;
         break;
      case ne:
         success = expected != actual;
         break;
      case ge:
         success = expected > actual;
         break;
      case gt:
         success = expected >= actual;
         break;
      default:
         break;
   }

   if (success)
   {
      _test_result.log_success();
      return;
   }

   _test_result.log_failure(expected, actual, args...);
}

template<bool ExpectedValue, typename... Args>
inline void Test::assert(bool value, Args... args)
{
   auto t = std::make_tuple(args...);

   if (has_type<option::Disabled>(t))
   {
      set_option(get_value<option::Disabled>(t, option::Disabled{}));

      _test_result.log_skipped(disabled_reason());
      return;
   }

   if (value == ExpectedValue)
   {
      _test_result.log_success();
      return;
   }

   _test_result.log_failure(ExpectedValue, value, args...);
}

template<typename ExpectedException, typename Func, typename... Args>
inline void Test::assert_throw(Func f, Args... args)
{
   try
   {
      f();
   }
   catch (const ExpectedException&)
   {
      _test_result.log_success();
      return;
   }
   catch (const std::exception& e)
   {
      _test_result.log_exception(e, "An unexpected exception was thrown: ", args...);
      return;
   }
   catch (...)
   {
      _test_result.log_exception(
         std::current_exception(), "An unexpected, unknown exception was thrown: ", args...);
      return;
   }
   // Not thrown
   fail("The exception was not thrown: ", type_name<ExpectedException>(), args...);
}

template<typename... Args>
inline void Test::fail(Args... args)
{
   _test_result.log_failure(args...);
}

template<typename... Args>
inline void Test::fail_if(bool value, Args... args)
{
   auto t = std::make_tuple(args...);

   if (has_type<option::Disabled>(t))
   {
      set_option(get_value<option::Disabled>(t, option::Disabled{}));

      _test_result.log_skipped(disabled_reason());
      return;
   }

   if (not value)
   {
      _test_result.log_success();
      return;
   }

   _test_result.log_failure(false, value, args...);
}

} // namespace unittest
} // namespace orion
#endif // ORION_UNITTEST_TEST_IPP