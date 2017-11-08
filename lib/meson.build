#
# Copyright (c) 2013-2017 Tomas Palazuelos
#
# Distributed under the MIT Software License. (See accompanying file LICENSE.md)
#
#--------------------------------------------------------------------------------------------------
# Lib orion
#

# Source files
orion_src = files(
   'AsyncService.cpp',
   'Encoding.cpp',
   'Object.cpp',
   'StringUtils.cpp',
   'System.cpp',
   # Logger files
   'log/EndRecord.h', 
   'log/ExceptionRecord.cpp', 
   'log/Function.cpp', 
   'log/Level.cpp', 
   'log/Logger.cpp', 
   'log/LoggerImpl.cpp', 
   'log/MultilineFormatter.cpp', 
   'log/OnelineFormatter.cpp', 
   'log/OnelineWithSourceInfoFormatter.cpp', 
   'log/OutputHandler.cpp', 
   'log/Record.cpp', 
   'log/StartRecord.h', 
   'log/StreamOutputHandler.cpp', 
   'log/SystemInfoRecord.h', 
   # Unit test files
   'unittest/Test.cpp', 
   'unittest/TestOutput.cpp', 
   'unittest/TestResult.cpp', 
   'unittest/TestRunner.cpp', 
   'unittest/TestStdOutput.cpp', 
   'unittest/TestSuite.cpp'
   )

orion_libs = [ boost_prog_opts_lib ]

if host_system == 'windows'
   orion_src += [
      'Module-win32.cpp',
      'System-win32.cpp',
      'Uuid-win32.cpp'
   ]

   orion_libs += [
      compiler.find_library('dbghelp'),
      compiler.find_library('ws2_32'),
      compiler.find_library('psapi'), 
      compiler.find_library('ntdll'), 
      compiler.find_library('rpcrt4') 
   ]
endif

if host_system == 'darwin'
   orion_src += [
      'Module-darwin.cpp',
      'System-darwin.cpp',
      'Uuid-darwin.cpp'
   ]
endif

if host_system == 'linux'
   orion_src += [
      'Module-linux.cpp',
      'System-linux.cpp',
      'Uuid-linux.cpp'
   ]
endif

orion_cflags = '-DORION_SHARED_EXPORTS'

#
# Build shared library
#
orion = shared_library('orion', 
                       sources: orion_src,
                       include_directories: global_includes, 
                       cpp_args: [orion_cflags, boost_cflags],
                       dependencies: [global_deps, orion_libs],
                       version : meson.project_version(), 
                       soversion : '0',
                       extra_files: orion_incl,
                       install: true)
#
# Dependency for tests and benchmarks and subproject usage
#
orion_dep = declare_dependency(link_with: orion,
                               dependencies: [global_deps, orion_libs])



#--------------------------------------------------------------------------------------------------
# Lib orion net
#

# Source files
orion_net_src = files(
   'net/Address.cpp',
   'net/AddressV4.cpp',
   'net/AddressV6.cpp',
   'net/EndPoint.cpp',
   'net/Error.cpp',
   'net/Url.cpp',
   # HTTP files
   'net/http/Error.cpp',
   'net/http/Parser.cpp',
   'net/http/Request.cpp',
   'net/http/Response.cpp',
   'net/http/Server.cpp',
   'net/http/ServerImpl.cpp',
   'net/http/ServerConnection.cpp',
   'net/http/Session.cpp',
   # TCP files
   'net/tcp/Server.cpp',
   'net/tcp/ServerImpl.cpp',
   'net/tcp/ServerConnection.cpp',
   'net/tcp/Session.cpp',
   'net/tcp/SessionImpl.cpp',
   'net/tcp/Utils.cpp'
   # RPC files
   )

if host_system == 'windows'
   orion_net_src += [
      'net/AddressV4-win32.cpp',
      'net/AddressV6-win32.cpp'
   ]

   orion_net_libs = [
      compiler.find_library('ws2_32'),
      compiler.find_library('mswsock'),
      compiler.find_library('psapi'), 
      compiler.find_library('ntdll'), 
      compiler.find_library('rpcrt4') 
   ]
endif

if host_system == 'darwin'
   orion_net_src += [
      'net/AddressV4-darwin.cpp',
      'net/AddressV6-darwin.cpp'
   ]
endif

orion_net_cflags = '-DORION_SHARED_EXPORTS'

orion_net_libs += [http_parser_dep, orion_dep]

#
# Build shared library
#
orion_net = shared_library('orion-net', 
                           sources: orion_net_src,
                           include_directories: global_includes, 
                           cpp_args: orion_net_cflags,
                           dependencies: [global_deps, orion_net_libs],
                           version : meson.project_version(), 
                           soversion : '0',
                           extra_files: orion_net_incl,
                           install: true)
#
# Dependency for tests and benchmarks and subproject usage
#
orion_net_dep = declare_dependency(link_with: orion_net,
                                   dependencies: [global_deps, orion_net_libs])