//
// Assert.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_ASSERT_H
#define ORION_ASSERT_H

#include <orion/Exception.h>
#include <orion/Macros.h>

//
// There are three configuration options for this GSL implementation's behavior
// when pre/post conditions on the GSL types are violated:
//
// 1. ORION_TERMINATE_ON_CONTRACT_VIOLATION: std::terminate will be called (default)
// 2. ORION_THROW_ON_CONTRACT_VIOLATION: an exception will be thrown
// 3. ORION_UNENFORCED_ON_CONTRACT_VIOLATION: nothing happens
//
#if !(defined(ORION_THROW_ON_CONTRACT_VIOLATION) ||     \
      defined(ORION_TERMINATE_ON_CONTRACT_VIOLATION) || \
      defined(ORION_UNENFORCED_ON_CONTRACT_VIOLATION))
#   define ORION_THROW_ON_CONTRACT_VIOLATION
#endif

#if !(defined(ORION_THROW_ON_ASSERT_VIOLATION) ||     \
      defined(ORION_TERMINATE_ON_ASSERT_VIOLATION) || \
      defined(ORION_UNENFORCED_ON_ASSERT_VIOLATION))
#   define ORION_THROW_ON_ASSERT_VIOLATION
#endif

namespace orion
{
//--------------------------------------------------------------------------------------------------
// Expects
// Ensures

class FailFast : public orion::Exception
{
public:
   FailFast(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

#if defined(ORION_TERMINATE_ON_CONTRACT_VIOLATION)

#   define ORION_CONTRACT_CHECK(type, cond) \
      (ORION_LIKELY(cond) ? static_cast<void>(0) : std::terminate())

#elif defined(ORION_THROW_ON_CONTRACT_VIOLATION)

#   define ORION_CONTRACT_CHECK(type, cond) \
      (ORION_LIKELY(cond) ? static_cast<void>(0) : throw FailFast(type, _src_loc))

#elif defined(ORION_UNENFORCED_ON_CONTRACT_VIOLATION)

#   define ORION_CONTRACT_CHECK(type, cond) ORION_ASSUME(cond)

#endif

#define Expects(cond) ORION_CONTRACT_CHECK("Precondition", cond)
#define Ensures(cond) ORION_CONTRACT_CHECK("Postcondition", cond)

//--------------------------------------------------------------------------------------------------
// AssertUnreachable

class AssertError : public orion::Exception
{
public:
   AssertError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

#if defined(ORION_TERMINATE_ON_ASSERT_VIOLATION)

#   define ORION_ASSERT_CHECK(type, cond) \
      (ORION_LIKELY(cond) ? static_cast<void>(0) : std::terminate())

#elif defined(ORION_THROW_ON_ASSERT_VIOLATION)

#   define ORION_ASSERT_CHECK(type, cond) \
      (ORION_LIKELY(cond) ? static_cast<void>(0) : throw AssertError(type, _src_loc))

#elif defined(ORION_UNENFORCED_ON_ASSERT_VIOLATION)

#   define ORION_ASSERT_CHECK(type, cond) ORION_ASSUME(cond)

#endif

#define AssertUnreachable(msg) ORION_ASSERT_CHECK("Unreachable code " msg, false)

} // namespace orion

#endif // ORION_ASSERT_H