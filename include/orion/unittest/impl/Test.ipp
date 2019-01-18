//
// Test.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TEST_IPP
#define ORION_UNITTEST_TEST_IPP

#include <orion/Assert.h>
#include <orion/Utils.h>

#include <string>

namespace orion
{
namespace unittest
{

template<class Func, typename T, typename... Args>
inline void Test::xassert(const T& expected, const T& actual, Args... args)
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
inline void Test::xassert(const T& expected, const T& actual, Args... args)
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
         success = expected >= actual;
         break;
      case gt:
         success = expected > actual;
         break;
      default:
         AssertUnreachable("Invalid operator");
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
inline void Test::xassert(bool value, Args... args)
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

template<typename... Args>
inline void Test::expected_exception_not_thrown(Args... args)
{
   _test_result.log_failure("Expected exception not thrown: ", args...);
}

template<typename ExceptionT, typename... Args>
inline void Test::exception_thrown_as_expected(const ExceptionT& e, Args... args)
{
   std::string msg{"Exception thrown as expected: "};

   msg += type_name(e);

   _test_result.log_success(msg, args...);
}

template<typename... Args>
inline void Test::exception_thrown_as_expected(std::exception_ptr eptr, Args... args)
{
   std::string msg{"Exception thrown as expected: "};

   msg += type_name(std::move(eptr));

   _test_result.log_success(msg, args...);
}

template<typename ExceptionT, typename... Args>
inline void Test::unexpected_exception_thrown(const ExceptionT& e, Args... args)
{
   _test_result.log_exception(e, "Unexpected exception thrown: ", args...);
}

template<typename... Args>
inline void Test::unexpected_exception_thrown(std::exception_ptr eptr, Args... args)
{
   _test_result.log_exception(eptr, "Unexpected exception thrown: ", args...);
}

} // namespace unittest
} // namespace orion
#endif // ORION_UNITTEST_TEST_IPP