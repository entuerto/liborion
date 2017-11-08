#
# Copyright (c) 2013-2017 Tomas Palazuelos
#
# Distributed under the MIT Software License. (See accompanying file LICENSE.md)
#
#--------------------------------------------------------------------------------------------------
# Tests for orion
#
orion_test_src = files(
   'test-encoding.cpp',
   'test-logger.cpp',
   'test-string.cpp',
   'test-units.cpp',
   'unittest/ClassTest.cpp',
   'unittest/ClassTestResult.cpp',
   'unittest/EvalMacros.cpp',
   'test-main.cpp'
   )

orion_test = executable('test-orion', 
                        orion_test_src,
                        dependencies: orion_dep,
                        include_directories: global_includes,
                        cpp_args: boost_cflags + ['-DORION_TEST_LOGGER'])

test('test-orion', orion_test)

#--------------------------------------------------------------------------------------------------
# Tests for orion-net
#
orion_net_test_src = files(
   'test-net.cpp',
   'test-url.cpp',
   'test-main.cpp'
   )

orion_net_test = executable('test-orion-net', 
                            orion_net_test_src,
                            dependencies: [orion_dep, orion_net_dep],
                            include_directories: global_includes,
                            cpp_args: boost_cflags)

test('test-orion-net', orion_net_test)

#--------------------------------------------------------------------------------------------------
# Tests for orion-rpc
#
#orion_rpc_test_src = files(
#   'test-jsonrpc.cpp',
#   'test-main.cpp'
#   )
#
#orion_rpc_test = executable('test-orion-rpc', 
#                            orion_rpc_test_src,
#                            dependencies: [orion_dep, orion_net_dep],
#                            include_directories: global_includes,
#                            cpp_args: boost_cflags)
#
#test('test-orion-rpc', orion_net_test)