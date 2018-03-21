#
# Copyright (c) 2013-2017 Tomas Palazuelos
#
# Distributed under the MIT Software License. (See accompanying file LICENSE.md)
#
project('orion', 'c', 'cpp',
   version : '0.1.0',
   license : 'MIT',
   default_options : [
      'b_pch=false',
      'cpp_std=c++14', 
      'warning_level=1'
   ])


compiler = meson.get_compiler('cpp')

host_system = host_machine.system()

#--------------------------------------------------------------------------------------------------
# Dependencies
#
# Asio
#
if compiler.has_header('asio.hpp', required: true) 
   asio_cflags = ['-DASIO_STANDALONE -DASIO_NO_DEPRECATED -DASIO_HAS_MOVE']

   if host_system == 'windows'
      asio_cflags += ['-D_WIN32_WINNT=0x0603', '-Wno-microsoft-enum-value']
   endif
   
   asio_dep = declare_dependency(compile_args: asio_cflags)
endif
#
# fmt library
#
fmt_lib = compiler.find_library('fmt', required: true)
#
# Boost program options
#
boost_prog_opts_lib = compiler.find_library('boost_program_options-mt', required: true)
if boost_prog_opts_lib.found()
   boost_cflags = [
      '-DBOOST_LIB_DIAGNOSTIC',
      '-DBOOST_ALL_NO_LIB',
      '-DBOOST_ALL_DYN_LINK'
   ]
endif

global_deps = [
   asio_dep,
   fmt_lib
]

global_cflags = [
  '-D_UNICODE',
  '-DUNICODE'
]

if host_system == 'windows'
   global_deps += [
      compiler.find_library('kernel32'),
      compiler.find_library('user32'),
      compiler.find_library('gdi32'),
      compiler.find_library('winspool'),
      compiler.find_library('shell32'),
      compiler.find_library('ole32'),
      compiler.find_library('oleaut32'),
      compiler.find_library('uuid'),
      compiler.find_library('comdlg32'),
      compiler.find_library('advapi32'),
      compiler.find_library('mincore', required: false),
      compiler.find_library('version', required: false)
   ]

   #add_global_arguments('-fms-compatibility-version=19', language: 'cpp')

   add_global_link_arguments('-Wl,--allow-multiple-definition', language: 'cpp')
endif

add_global_arguments('-Wshadow', '-Wundef', language: 'cpp')

#--------------------------------------------------------------------------------------------------
# Project include paths
#
global_includes = [
  include_directories('include', 'lib', 'deps')
]

subdir('deps')
subdir('include')
subdir('lib')

#--------------------------------------------------------------------------------------------------
# Unit tests
if get_option('enable-tests')
  subdir('tests')
endif

#--------------------------------------------------------------------------------------------------
# Examples
if get_option('enable-examples')
  subdir('examples')
endif