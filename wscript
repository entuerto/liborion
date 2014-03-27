#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
This is a WAF build script (http://code.google.com/p/waf/).

Requires WAF 1.7.13 and Python 2.7 (or later).
"""

from waflib import Context, Options, Scripting, Configure, Utils, Logs
from waflib.Configure import ConfigurationContext
from waflib.Errors import WafError
from waflib.Tools import waf_unit_test
from waflib.Tools.compiler_c import c_compiler
from waflib.Tools.compiler_cxx import cxx_compiler
import sys
import os
import posixpath
import tempfile

major = 0
minor = 1
micro = 0

APPNAME = 'liborion'
VERSION = str (major) + '.' + str (minor) + '.' + str (micro)
LIBORION_API_VERSION = str (major) + '.' + str (minor)

DEPENDENCIES = ''

top = '.'
out = 'build'

mongoose_sources = ['lib/ws/mongoose/mongoose.c']

json_sources = ['lib/ws/json/json_reader.cpp',
                'lib/ws/json/json_value.cpp',
                'lib/ws/json/json_writer.cpp']

plugin_sources = ['lib/plugin/Extension.cpp',
                  'lib/plugin/PlugIn.cpp',
                  'lib/plugin/PlugInException.cpp',
                  'lib/plugin/PlugInManager.cpp']

ws_sources =  ['lib/ws/mongoose/MongooseRequest.cpp',
               'lib/ws/mongoose/MongooseHttpServer.cpp',
               'lib/ws/InetAddress.cpp',
               'lib/ws/RpcError.cpp',
               'lib/ws/RpcMethod.cpp',
               'lib/ws/JsonRpcError.cpp',
               'lib/ws/JsonRpcMethodWrapper.cpp',
               'lib/ws/Request.cpp',
               'lib/ws/RequestListener.cpp',
               'lib/ws/RpcRequestListener.cpp',
               'lib/ws/JsonRpcRequestListener.cpp',
               'lib/ws/Response.cpp',
               'lib/ws/HttpServer.cpp']

core_sources = ['lib/ArgumentExceptions.cpp',
                'lib/DateUtils.cpp',
                'lib/Exception.cpp',
                'lib/Module.cpp',
                'lib/ModuleException.cpp',
                'lib/NotImplementedException.cpp',
                'lib/StringUtils.cpp',
                'lib/SystemInfo.cpp',
                'lib/Timer.cpp',
                'lib/fs/FileUtils.cpp',
                'lib/logging/LogEndRecord.cpp',
                'lib/logging/LogExceptionRecord.cpp',
                'lib/logging/LogFunction.cpp',
                'lib/logging/LogRecord.cpp',
                'lib/logging/LogStartRecord.cpp',
                'lib/logging/LogSystemInfoRecord.cpp',
                'lib/logging/Logger.cpp',
                'lib/logging/MultilineFormatter.cpp',
                'lib/logging/OnelineFormatter.cpp',
                'lib/logging/OnelineWithSourceInfoFormatter.cpp',
                'lib/logging/OutputHandler.cpp',
                'lib/logging/StreamOutputHandler.cpp',
                'lib/unittest/Test.cpp',
                'lib/unittest/TestResult.cpp',
                'lib/unittest/TestResultItem.cpp',
                'lib/unittest/TestStdOutput.cpp']

debug_level = {'none'  : ['-O2'],
               'debug' : ['-g', '-O2'],
               'full'  : ['-g']
              }

warning_level = {'none'  : [],
                 'min'   : ['-Wall'],
                 'max'   : ['-Wall', '-Wextra', '-Wno-unused-parameter', '-Winit-self', '-Wno-sign-compare'],
                 'fatal' : ['-Wall', '-Wextra', '-Werror']
                }


def options(opt):
   # Disable MSVC detection on win32: building with MSVC is currently not supported
   # If anyone wants to add support for building with MSVC, this hack should be removed.
   c_compiler['win32'] = ['gcc']
   cxx_compiler['win32'] = ['g++']

   opt.load('compiler_c')
   opt.load('compiler_cxx')
   opt.load('gnu_dirs')
   opt.load('waf_unit_test')

   opt.parser.remove_option('--oldincludedir')
   opt.parser.remove_option('--localstatedir')
   opt.parser.remove_option('--sharedstatedir')
   opt.parser.remove_option('--htmldir')
   opt.parser.remove_option('--dvidir')
   opt.parser.remove_option('--infodir')
   opt.parser.remove_option('--mandir')
   opt.parser.remove_option('--pdfdir')
   opt.parser.remove_option('--psdir')

   test_option_group = opt.add_option_group('Test options')

   test_option_group.add_option('--run-tests',
                                action  = 'store_true',
                                default = False,
                                help    = 'compile and run tests [default: No]',
                                dest    = 'run_tests')

   test_option_group.add_option('--compile-tests',
                                action  = 'store_true',
                                default = False,
                                help    = 'compile tests [default: No]',
                                dest    = 'compile_tests')

   ex_option_group = opt.add_option_group('Examples options')

   ex_option_group.add_option('--compile-examples',
                              action  = 'store_true',
                              default = False,
                              help    = 'compile examples [default: No]',
                              dest    = 'compile_examples')

   cxx_option_group = opt.get_option_group('C++ Compiler Options')

   cxx_option_group.add_option('--debug-level',
                               action  = 'store',
                               default = 'none',
                               help    = 'Specify the debugging level. [\'none\', \'debug\', \'full\']',
                               choices = ['', 'none', 'debug', 'full'],
                               dest    = 'debug_level')

   cxx_option_group.add_option('--warning-level',
                               action  = 'store',
                               default = 'none',
                               help    = 'Specify the warning level. [\'none\', \'min\', \'max\', \'fatal\']',
                               choices = ['', 'none', 'min', 'max', 'fatal'],
                               dest    = 'warning_level')


def configure(conf):
   conf.check_waf_version(mini='1.6.1')

   conf.load('compiler_c')
   conf.load('compiler_cxx')
   conf.load('waf_unit_test')

   is_win32 = _target_is_win32(conf)  

   # This is specific to compiling with mingw
   if is_win32:
      # overwrite default prefix on Windows (tempfile.gettempdir() is the Waf default)
      if conf.options.prefix.lower() == tempfile.gettempdir().lower():
         conf.options.prefix = os.path.join(os.path.abspath(top), '%s-%s' % (APPNAME, VERSION))
         _add_value(conf, 'PREFIX', conf.options.prefix, quote=True)

      # Make sure we don't have -fPIC in the CCFLAGS
      conf.env["shlib_CCFLAGS"] = []

      # Adjust file naming
      conf.env["cshlib_PATTERN"]   = 'lib%s.dll'
      conf.env["cxxshlib_PATTERN"] = 'lib%s.dll'
      conf.env['program_PATTERN']  = '%s.exe'

      # Use Visual C++ compatible alignment
      conf.env.append_value ('CCFLAGS', '-mms-bitfields')
      conf.env['staticlib_LINKFLAGS'] = []

   # Load gnu_dirs here because we modify PREFIX for win32
   conf.load('gnu_dirs')
   
   # uuid
   if not is_win32:
      conf.check_cfg(package         = 'uuid',
                     atleast_version = '2.16',
                     uselib_store    = 'UUID',
                     mandatory       = False,
                     args            = '--cflags --libs')
      uuid_version = conf.check_cfg(modversion='uuid', uselib_store='UUID', mandatory=False)


   conf.check(header_name='sys/time.h')
   conf.check(header_name='sys/types.h')
   conf.check(header_name='sys/stat.h')

   conf.define('PACKAGE', APPNAME)
   conf.define('VERSION', VERSION)

   conf.define('PACKAGE_NAME', APPNAME)
   conf.define('PACKAGE_VERSION', VERSION)
   conf.define('PACKAGE_BUGREPORT', 'tomasp@videotron.ca')
   conf.define('GETTEXT_PACKAGE', APPNAME)

   conf.define('LIBORION_MAJOR_VERSION', major)
   conf.define('LIBORION_MINOR_VERSION', minor)
   conf.define('LIBORION_MICRO_VERSION', micro)

   _add_value(conf, 'LIBORION_PREFIX', conf.env['PREFIX'])
   _add_value(conf, 'LIBORION_LIBDIR', conf.env['LIBDIR'])
   _add_value(conf, 'LIBORION_DATADIR', conf.env['DATADIR'])
   _add_value(conf, 'LIBORION_DOCDIR', conf.env.DOCDIR)
   _add_value(conf, 'LIBORION_SYSTEM_CONFIG_DIR', conf.env['SYSCONFDIR'])

   conf.write_config_header('config.h')

   conf.env.append_value('CPPFLAGS', '-DHAVE_CONFIG_H')

   conf.env['CXXFLAGS'] = debug_level[Options.options.debug_level]
   conf.env.append_value('CXXFLAGS', warning_level[Options.options.warning_level])
   conf.env.append_value('CXXFLAGS', '-std=c++11')

   if _target_is_darwin(conf):
      conf.env.append_value('CXXFLAGS', '-stdlib=libc++')

   if is_win32:
      conf.env.append_value('CXXFLAGS', '-fpermissive')
      conf.env.append_value('LDFLAGS', '-no-undefined')

   _print(conf, "")
   _print(conf, "liborion {0}".format(VERSION))
   _print(conf, "")
   _print(conf, "Configuration:")
   _print(conf, "")
   _print(conf, "  prefix                  : {0}".format(conf.env['PREFIX']))
   _print(conf, "  datadir                 : {0}".format(conf.env['DATADIR']))
   _print(conf, "  libdir                  : {0}".format(conf.env['LIBDIR']))
   _print(conf, "  includedir              : {0}".format(conf.env['INCLUDEDIR']))
   _print(conf, "  Source code location    : {0}".format(conf.top_dir))
   _print(conf, "  C Compiler              : {0}".format(conf.env['CC']))
   _print(conf, "  C++ Compiler            : {0}".format(conf.env['CXX']))
   _print(conf, "  CFLAGS                  : {0}".format(conf.env['CFLAGS']))
   _print(conf, "  CXXFLAGS                : {0}".format(conf.env['CXXFLAGS']))
   _print(conf, "  CPPFLAGS                : {0}".format(conf.env['CPPFLAGS']))
   _print(conf, "  Uuid version            : {0}".format(uuid_version if not is_win32 else 'Windows rpc'))


def build(bld):
   is_win32 = _target_is_win32(bld)
   is_darwin = _target_is_darwin(bld)

   if is_win32:
      core_sources.append('lib/Module-Win32.cpp')
      core_sources.append('lib/SystemInfo-Win32.cpp')
      core_sources.append('lib/Uuid-win32.cpp')
   elif is_darwin:
      core_sources.append('lib/SystemInfo-osx.cpp')
      core_sources.append('lib/Uuid.cpp')
   else:
      core_sources.append('lib/Module-linux.cpp')
      core_sources.append('lib/SystemInfo-linux.cpp')
      core_sources.append('lib/Uuid.cpp')
   
   obj = bld.stlib(target   = 'mongoose',
                   features = 'c cstlib',
                   source   = mongoose_sources,
                   includes  = ['.'],
                   lib       = '',
                   uselib    = '')

   obj = bld.stlib(target    = 'json',
                   features  = 'cxx cxxstlib',
                   source    = json_sources,
                   includes  = ['.', 'lib/ws'],
                   lib       = 'c++' if is_darwin else '',
                   uselib    = '')

   obj = bld.shlib(target    = 'orion',
                   features  = 'cxx cxxshlib',
                   source    = core_sources,
                   includes  = ['.', 'include/', 'lib/'],
                   lib       = 'c++' if is_darwin else '',
                   uselib    = '')

   if is_win32:
      obj.lib = ['psapi', 'rpcrt4', 'ntdll']

   obj = bld.shlib(target    = 'orion-ws',
                   features  = 'cxx cxxshlib',
                   source    = ws_sources,
                   includes  = ['.', 'include/', 'lib/'],
                   lib       = 'c++' if is_darwin else '',
                   uselib    = '',
                   use       = ['orion', 'json', 'mongoose'])

   if is_win32:
      obj.lib = ['psapi', 'rpcrt4', 'ws2_32']

   obj = bld.shlib(target    = 'orion-plugin',
                   features  = 'cxx cxxshlib',
                   source    = plugin_sources,
                   includes  = ['.', 'include/', 'lib/'],
                   lib       = 'c++' if is_darwin else '',
                   uselib    = '',
                   use       = ['orion'])

   obj = bld(features     = 'subst',
             source       = 'liborion.pc.in',
             target       = 'liborion-' + LIBORION_API_VERSION + '.pc',
             dct          = {'VERSION' : VERSION,
                             'DEPENDENCIES' : DEPENDENCIES,
                             'prefix': bld.env['PREFIX'],
                             'exec_prefix': '${prefix}',
                             'libdir': '${exec_prefix}/lib',
                             'includedir': '${prefix}/include',
                             'datarootdir': '${prefix}/share',
                             'datadir': '${datarootdir}',
                             'localedir': '${datarootdir}/locale',
                             'LIBORION_API_VERSION': LIBORION_API_VERSION},
             install_path = '${LIBDIR}/pkgconfig')

   obj = bld(features     = 'subst',
             source       = 'liborion-ws.pc.in',
             target       = 'liborion-ws-' + LIBORION_API_VERSION + '.pc',
             dct          = {'VERSION' : VERSION,
                             'DEPENDENCIES' : DEPENDENCIES + 'liborion-' + LIBORION_API_VERSION,
                             'prefix': bld.env['PREFIX'],
                             'exec_prefix': '${prefix}',
                             'libdir': '${exec_prefix}/lib',
                             'includedir': '${prefix}/include',
                             'datarootdir': '${prefix}/share',
                             'datadir': '${datarootdir}',
                             'localedir': '${datarootdir}/locale',
                             'LIBORION_API_VERSION': LIBORION_API_VERSION},
             install_path = '${LIBDIR}/pkgconfig')

   obj = bld(features     = 'subst',
             source       = 'liborion-plugin.pc.in',
             target       = 'liborion-plugin-' + LIBORION_API_VERSION + '.pc',
             dct          = {'VERSION' : VERSION,
                             'DEPENDENCIES' : DEPENDENCIES + 'liborion-' + LIBORION_API_VERSION,
                             'prefix': bld.env['PREFIX'],
                             'exec_prefix': '${prefix}',
                             'libdir': '${exec_prefix}/lib',
                             'includedir': '${prefix}/include',
                             'datarootdir': '${prefix}/share',
                             'datadir': '${datarootdir}',
                             'localedir': '${datarootdir}/locale',
                             'LIBORION_API_VERSION': LIBORION_API_VERSION},
             install_path = '${LIBDIR}/pkgconfig')

   bld.install_files('${PREFIX}/include/orion-' + LIBORION_API_VERSION + '/orion', bld.path.ant_glob('include/orion/*'))
   bld.install_files('${PREFIX}/include/orion-' + LIBORION_API_VERSION + '/orion/logging', bld.path.ant_glob('include/orion/logging/*'))
   bld.install_files('${PREFIX}/include/orion-' + LIBORION_API_VERSION + '/orion/unittest', bld.path.ant_glob('include/orion/unittest/*'))
   bld.install_files('${PREFIX}/include/orion-' + LIBORION_API_VERSION + '/orion/ws', bld.path.ant_glob('include/orion/ws/*'))
   bld.install_files('${PREFIX}/include/orion-' + LIBORION_API_VERSION + '/orion/plugin', bld.path.ant_glob('include/orion/plugin/*'))

   # Tests
   if Options.options.compile_tests or Options.options.run_tests:
      bld.program(
          target       = 'test-logger',
          features     = 'cxx cprogram',
          source       = ['tests/test-logger.cpp'],
          includes     = ['.', 'tests/', 'include/'],
          use          = ['orion'],
          lib          = 'c++' if is_darwin else '',
          install_path = None,
          unit_test    = 1)

      bld.program(
          target       = 'test-unittest',
          features     = 'cxx cprogram',
          source       = ['tests/test-unittest.cpp',
                          'tests/unittest/ClassTest.cpp',
                          'tests/unittest/ClassTestResult.cpp',
                          'tests/unittest/EvalMacros.cpp'],
          includes     = ['.', 'tests/', 'include/'],
          use          = ['orion'],
          lib          = 'c++' if is_darwin else '',
          install_path = None,
          unit_test    = 1)

      bld.program(
          target       = 'test-jsonrpc',
          features     = 'cxx cprogram',
          source       = ['tests/test-jsonrpc.cpp'],
          includes     = ['.', 'tests/', 'include/', 'lib/'],
          use          = ['orion-ws', 'orion'],
          lib          = 'c++' if is_darwin else '',
          install_path = None,
          unit_test    = 1)

      bld.program(
          target       = 'test-string',
          features     = 'cxx cprogram',
          source       = ['tests/test-string.cpp'],
          includes     = ['.', 'tests/', 'include/', 'lib/'],
          use          = ['orion'],
          lib          = 'c++' if is_darwin else '',
          install_path = None,
          unit_test    = 1)

   # Examples
   if Options.options.compile_examples:
      bld.program(
          target       = 'hello-server',
          features     = 'cxx cprogram',
          source       = ['examples/hello-server.cpp'],
          includes     = ['.', 'examples/', 'include/'],
          use          = ['orion-ws', 'orion'],
          lib          = 'c++' if is_darwin else '',
          install_path = None)

      bld.program(
          target       = 'add-json-rpc-server',
          features     = 'cxx cprogram',
          source       = ['examples/add-json-rpc-server.cpp'],
          includes     = ['.', 'examples/', 'include/', 'lib/'],
          use          = ['orion-ws', 'orion'],
          lib          = 'c++' if is_darwin else '',
          install_path = None)

      bld.program(
          target       = 'system-info',
          features     = 'cxx cprogram',
          source       = ['examples/system-info.cpp'],
          includes     = ['.', 'examples/', 'include/', 'lib/'],
          use          = ['orion'],
          lib          = 'c++' if is_darwin else '',
          install_path = None)

      bld.program(
          target       = 'signal-example',
          features     = 'cxx cprogram',
          source       = ['examples/signal-example.cpp'],
          includes     = ['.', 'examples/', 'include/', 'lib/'],
          use          = ['orion'],
          lib          = 'c++' if is_darwin else '',
          install_path = None)

      bld.program(
          target       = 'log-example',
          features     = 'cxx cprogram',
          source       = ['examples/log-example.cpp'],
          includes     = ['.', 'examples/', 'include/', 'lib/'],
          use          = ['orion'],
          lib          = 'c++' if is_darwin else '',
          install_path = None)

      obj = bld.shlib(target    = 'module-example',
                      features  = 'cxx cxxshlib',
                      source    = ['examples/module-example-lib.cpp'],
                      includes  = ['.', 'examples/', 'include/', 'lib/'],
                      use       = ['orion'],
                      lib       = 'c++' if is_darwin else '',
                      uselib    = '')

      bld.program(
          target       = 'module-example',
          features     = 'cxx cprogram',
          source       = ['examples/module-example.cpp'],
          includes     = ['.', 'examples/', 'include/', 'lib/'],
          use          = ['orion'],
          lib          = 'c++' if is_darwin else '',
          install_path = None)

   bld.add_post_fun(summary)


def summary(ctx):
   if Options.options.run_tests:
      ctx.exec_command(os.path.join(ctx.out_dir, 'test-logger.exe'))
      ctx.exec_command(os.path.join(ctx.out_dir, 'test-unittest.exe'))
      ctx.exec_command(os.path.join(ctx.out_dir, 'test-jsonrpc.exe'))
      ctx.exec_command(os.path.join(ctx.out_dir, 'test-string.exe'))


def distclean(ctx):
   Scripting.distclean(ctx)

   try:
      os.unlink("docs/html")
   except OSError:
      pass
    
   try:
      os.mkdir(out) 
   except OSError:
      pass


def apidoc(ctx):
    """generate API reference documentation"""
    basedir = ctx.path.abspath()
    doxygen = 'doxygen'
    doxyfile = os.path.join(basedir, 'docs', 'Doxyfile')

    os.chdir('docs')

    _print(ctx, 'Generating API documentation')

    ret = ctx.exec_command('%s %s' % (doxygen, doxyfile))
    if ret != 0:
        raise WafError('Generating API documentation failed')

    os.chdir('..')


def list_targets(ctx):
   bld = Build.BuildContext()

   proj = Environment.Environment(Options.lockfile)
   bld.load_dirs(proj['srcdir'], proj['blddir'])
   bld.load_envs()

   bld.add_subdirs([os.path.split(Utils.g_module.root_path)[0]])

   print 'targets to build: '

   for x in bld.all_task_gen:
      try:
         print ' -- ', x.name or x.target
      except AttributeError:
         pass


def _define(conf, key, value):
   value = value.replace('\\', posixpath.sep) if _target_is_win32(conf) else value
   conf.define(key, value)


def _add_value(conf, key, value, quote=False):
   value = value.replace('\\', posixpath.sep) if _target_is_win32(conf) else value
   conf.define(key, value, quote)
   conf.env[key] = value


def _target_is_win32(ctx):
   if "is_win32" in ctx.env:
      # cached
      return ctx.env["is_win32"]

   is_win32 = None
   if sys.platform == "win32":
      is_win32 = True

   if is_win32 is None:
      if ctx.env and 'CC' in ctx.env:
         env_cc = ctx.env['CC']
         if not isinstance(env_cc, str):
            env_cc = ''.join(env_cc)
         is_win32 = (env_cc.find('mingw') != -1)

   if is_win32 is None:
      is_win32 = False

   # cache for future checks
   ctx.env["is_win32"] = is_win32
   return is_win32


def _target_is_darwin(ctx):
   if sys.platform == 'darwin':
      return True


def _print(ctx, msg):
   ctx.to_log(msg)
   Logs.pprint('NORMAL', msg)

