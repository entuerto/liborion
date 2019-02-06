//
// TestBase.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTBASE_IPP
#define ORION_UNITTEST_TESTBASE_IPP

#include <fmt/format.h>

namespace orion
{
namespace unittest
{

inline TestInfo::TestInfo(std::string name)
   : _name(std::move(name))
   , _label{_name}
{
}

inline std::string TestInfo::name() const
{
   return _name;
}

inline Label TestInfo::label() const
{
   return _label;
}

inline Description TestInfo::description() const
{
   return _description;
}

inline Enabled TestInfo::enabled() const
{
   return _enabled;
}

inline Disabled TestInfo::disabled() const
{
   return _disabled;
}

inline void TestInfo::set_option(Label opt)
{
   _label = std::move(opt);
}

inline void TestInfo::set_option(Description opt)
{
   _description = std::move(opt);
}

inline void TestInfo::set_option(SetupFunc opt)
{
   _setup_func = std::move(opt);
}

inline void TestInfo::set_option(TeardownFunc opt)
{
   _teardown_func = std::move(opt);
}

inline void TestInfo::set_option(Enabled opt)
{
   _enabled = opt;
}

inline void TestInfo::set_option(Disabled opt)
{
   _enabled = Enabled{false};
   _disabled = std::move(opt);
}

inline void TestInfo::invoke_setup_func() const
{
   if (_setup_func.value)
   {
      _setup_func.value();
   }
}

inline void TestInfo::invoke_teardown_func() const
{
   if (_teardown_func.value)
   {
      _teardown_func.value();
   }
}

} // namespace unittest
} // namespace orion

//--------------------------------------------------------------------------------------------------
// Formatters for fmtlib

namespace fmt
{
template<>
struct formatter<orion::unittest::Label>
{
   template<typename ParseContext>
   constexpr auto parse(ParseContext& ctx)
   {
      return ctx.begin();
   }

   template<typename FormatContext>
   auto format(const orion::unittest::Label& l, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", l.value);
   }
};

template<>
struct formatter<orion::unittest::Description>
{
   template<typename ParseContext>
   constexpr auto parse(ParseContext& ctx)
   {
      return ctx.begin();
   }

   template<typename FormatContext>
   auto format(const orion::unittest::Description& d, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", d.value);
   }
};

} // namespace fmt

#endif // ORION_UNITTEST_TESTBASE_IPP
