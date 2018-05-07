#
# Copyright (c) 2013-2017 Tomas Palazuelos
#
# Distributed under the MIT Software License. (See accompanying file LICENSE.md)
#
#--------------------------------------------------------------------------------------------------
# Include files
#

_base_incl = files(
   'orion/ART.h',
   'orion/AsyncService.h',
   'orion/Chrono.h',
   'orion/Config.h',
   'orion/Datetime.h',
   'orion/Encoding.h',
   'orion/ErrorMacros.h',
   'orion/Exception.h',
   'orion/Iterator.h',
   'orion/Log.h',
   'orion/Memory.h',
   'orion/Module.h',
   'orion/Orion-Stddefs.h',
   'orion/Signal.h',
   'orion/SourceLocation.h',
   'orion/String.h',
   'orion/System.h',
   'orion/Test.h',
   'orion/Throw.h',
   'orion/Units.h',
   'orion/Utils.h',
   'orion/Uuid.h'
   )

_base_impl_incl = files(
   'orion/impl/ART.ipp',
   'orion/impl/Chrono.ipp',
   'orion/impl/Datetime.ipp',
   'orion/impl/Exception.ipp'
   )

_config_incl = files(
   'orion/config/Bsd.h',
   'orion/config/Cygwin.h',
   'orion/config/Linux.h',
   'orion/config/Macos.h',
   'orion/config/Posix.h',
   'orion/config/Windows.h'
   )

_log_incl = files(
   'orion/log/BasicLogger.h',
   'orion/log/ExceptionRecord.h',
   'orion/log/Formatter.h',
   'orion/log/Function.h',
   'orion/log/Level.h',
   'orion/log/Logger.h',
   'orion/log/LoggerImpl.h',
   'orion/log/LoggerService.h',
   'orion/log/Macros.h',
   'orion/log/OutputHandler.h',
   'orion/log/Record.h'
   )

_log_impl_incl = files(
   'orion/log/impl/BasicLogger.ipp',
   'orion/log/impl/Logger.ipp'
   )

_string_incl = files(
   'orion/string/Compare.h',
   'orion/string/Finder.h',
   'orion/string/Join.h',
   'orion/string/Split.h',
   'orion/string/Transform.h',
   'orion/string/Trim.h'
   )

_unittest_incl = files(
   'orion/unittest/Test.h',
   'orion/unittest/TestMacros.h',
   'orion/unittest/TestOptions.h',
   'orion/unittest/TestOutput.h',
   'orion/unittest/TestResult.h',
   'orion/unittest/TestRunner.h',
   'orion/unittest/TestStdOutput.h',
   'orion/unittest/TestSuite.h'
   )

_unittest_impl_incl = files(
   'orion/unittest/impl/Test.ipp',
   'orion/unittest/impl/TestResult.ipp'
   )

orion_incl = [
   _base_incl, 
   _base_impl_incl, 
   _config_incl, 
   _log_incl, 
   _log_impl_incl, 
   _string_incl,
   _unittest_incl, 
   _unittest_impl_incl
   ]

install_headers(_base_incl, subdir : meson.project_name())
install_headers(_base_impl_incl, subdir : join_paths(meson.project_name(), 'impl'))
install_headers(_config_incl, subdir : join_paths(meson.project_name(), 'config'))
install_headers(_log_incl, subdir : join_paths(meson.project_name(), 'log'))
install_headers(_log_impl_incl, subdir : join_paths(meson.project_name(), 'log', 'impl'))
install_headers(_string_incl, subdir : join_paths(meson.project_name(), 'string'))
install_headers(_unittest_incl, subdir : join_paths(meson.project_name(), 'unittest'))
install_headers(_unittest_impl_incl, subdir : join_paths(meson.project_name(), 'unittest', 'impl'))

#---------------------------------------------------------------------------------------------------
#  Network
#---------------------------------------------------------------------------------------------------

_net_incl = files(
   'orion/net/Address.h',
   'orion/net/AddressV4.h',
   'orion/net/AddressV6.h',
   'orion/net/Connection.h',
   'orion/net/EndPoint.h',
   'orion/net/Error.h',
   'orion/net/Listener.h',
   'orion/net/Types.h',
   'orion/net/Url.h',
   'orion/net/Utils.h'
   )

_net_impl_incl = files(
   'orion/net/impl/Connection.ipp'
   )

_http_incl = files(
   'orion/net/http/Client.h',
   'orion/net/http/Cookies.h',
   'orion/net/http/Error.h',
   'orion/net/http/Request.h',
   'orion/net/http/RequestMux.h',
   'orion/net/http/Response.h',
   'orion/net/http/Server.h',
   'orion/net/http/Session.h',
   'orion/net/http/StatusCode.h',
   'orion/net/http/Utils.h'
   )

_http_impl_incl = files(
   'orion/net/http/impl/StatusCode.ipp',
   'orion/net/http/impl/Utils.ipp'
   )

_rpc_incl = files(
   'orion/net/rpc/Error.h',
   'orion/net/rpc/Method.h',
   'orion/net/rpc/Protocol.h',
   'orion/net/rpc/Result.h',
   'orion/net/rpc/Rpc.h',
   'orion/net/rpc/Serializer.h',
   'orion/net/rpc/Service.h',
   'orion/net/rpc/Traits.h'
   )

_rpc_impl_incl = files(
   'orion/net/rpc/impl/Protocol.ipp',
   'orion/net/rpc/impl/Service.ipp'
   )

_tcp_incl = files(
   'orion/net/tcp/Connection.h',
   'orion/net/tcp/Listener.h',
   'orion/net/tcp/Session.h',
   'orion/net/tcp/Utils.h'
   )

_tcp_impl_incl = files(
   'orion/net/tcp/impl/Connection.ipp',
   'orion/net/tcp/impl/Listener.ipp'
   )

orion_net_incl = [
   _net_incl,
   _net_impl_incl,
   _http_incl,
   _http_impl_incl,
   _rpc_incl,
   _rpc_impl_incl,
   _tcp_incl,
   _tcp_impl_incl
   ]

install_headers(_net_incl, subdir : join_paths(meson.project_name(), 'net'))
install_headers(_net_impl_incl, subdir : join_paths(meson.project_name(), 'net', 'impl'))
install_headers(_http_incl, subdir : join_paths(meson.project_name(), 'net', 'http'))
install_headers(_http_impl_incl, subdir : join_paths(meson.project_name(), 'net', 'http', 'impl'))
install_headers(_rpc_incl, subdir : join_paths(meson.project_name(), 'net', 'rpc'))
install_headers(_rpc_impl_incl, subdir : join_paths(meson.project_name(), 'net', 'rpc', 'impl'))
install_headers(_tcp_incl, subdir : join_paths(meson.project_name(), 'net', 'tcp'))
install_headers(_tcp_impl_incl, subdir : join_paths(meson.project_name(), 'net', 'tcp', 'impl'))
