//
// TestBase.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTBASE_IPP
#define ORION_UNITTEST_TESTBASE_IPP

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
   _enabled = std::move(opt);
}

inline void TestInfo::set_option(Disabled opt)
{
   _enabled = Enabled{false};
   _disabled = std::move(opt);
}

inline void TestInfo::invoke_setup_func() const
{
   if (_setup_func.value)
      _setup_func.value();
}

inline void TestInfo::invoke_teardown_func() const
{
   if (_teardown_func.value)
      _teardown_func.value();
}

} // namespace unittest
} // namespace orion
#endif // ORION_UNITTEST_TESTBASE_IPP
