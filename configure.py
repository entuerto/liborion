#!/usr/bin/env python

"""
Script that generates the build.ninja for ninja itself.
Projects that use ninja themselves should either write a similar script
or use a meta-build system that supports Ninja output.
"""

from optparse import OptionParser
import os
import pipes
import re
import string
import subprocess
import sys

root_dir = os.path.dirname(os.path.realpath(__file__))
sys.path.insert(0, os.path.join(root_dir, 'scripts'))

import ninja_syntax

BUILD_FILENAME = 'build.ninja'
BIN_OUT_NAME = 'bin'
LIB_OUT_NAME = 'lib'
OBJ_OUT_NAME = 'obj'


class Platform(object):
   """Represents a host/target platform and its specific build attributes."""
   def __init__(self, platform):
      self._platform = platform
      if self._platform is not None:
         return

      self._platform = sys.platform
      if self._platform.startswith('linux'):
         self._platform = 'linux'
      elif self._platform.startswith('freebsd'):
         self._platform = 'freebsd'
      elif self._platform.startswith('openbsd'):
         self._platform = 'openbsd'
      elif self._platform.startswith('mingw') or os.environ.get('MSYSTEM', '').startswith('MINGW'):
         self._platform = 'mingw' 
      elif self._platform.startswith('win'):
         self._platform = 'msvc'
      elif self._platform.startswith('netbsd'):
         self._platform = 'netbsd'

      if self.is_msvc():
         self._sysname = 'windows'  
      else: 
         self._sysname = self._platform

   @staticmethod
   def known_platforms():
      return ['linux', 'darwin', 'freebsd', 'mingw', 'msvc']

   def platform(self):
      return self._platform

   def sysname(self):
      return self._sysname

   def is_darwin(self):
      return self._platform == 'darwin'

   def is_linux(self):
      return self._platform == 'linux'

   def is_mingw(self):
      return self._platform == 'mingw'

   def is_msvc(self):
      return self._platform == 'msvc'

   def is_windows(self):
      return self.is_mingw() or self.is_msvc()

   def uses_usr_local(self):
      return self._platform in ('freebsd', 'linux')

  
#---------------------------------------------------------------------------------------------------
# Default compiler and linker Flags

CFLAGS   = os.environ.get('CFLAGS', '').split()
CPPFLAGS = os.environ.get('CPPFLAGS', '').split()
CXXFLAGS = os.environ.get('CXXFLAGS', '').split()
LDFLAGS  = os.environ.get('LDFLAGS', '').split()
LIBFLAGS = os.environ.get('LIBFLAGS', '').split()

windows_libs = [
   'mincore',
   'kernel32', 
   'user32', 
   'shell32', 
   'advapi32', 
   'mswsock', 
   'ntdll', 
   'psapi', 
   'rpcrt4', 
   'ws2_32'
]

mingw_windows_libs = ['mincore', 'mswsock', 'ntdll', 'psapi', 'rpcrt4', 'ws2_32']

# -Wno-c++11-narrowing
default_warning = ['-Wshadow', '-Wundef', '-Wnon-virtual-dtor']

default_windows_defines = [
   '-DUNICODE', 
   '-D_UNICODE', 
   '-DWIN32_LEAN_AND_MEAN', 
   '-DWINVER=0x0A00', 
   '-D_WIN32_WINNT=0x0A00',
   '-D_CRT_SECURE_NO_DEPRECATE',
   '-D_SCL_SECURE_NO_DEPRECATE'
   ]

buildtypes = ['debug', 'debugoptimized', 'release', 'minsize']

gnu_comp_args = {
   'debug': {
      'defines'  : ['-DDEBUG'],
      'cflags'   : ['-O0', '-g'],
      'cxxflags' : ['-O0', '-g'],
   },
   'debugoptimized': {
      'defines'  : ['-DDEBUG'],
      'cflags'   : ['-O1', '-g'],
      'cxxflags' : ['-O1', '-g'],
   },
   'release': {
      'defines'  : ['-DNDEBUG'],
      'cflags'   : ['-Os'],
      'cxxflags' : ['-Os'],
   },
   'minsize': {
      'defines'  : ['-DNDEBUG'],
      'cflags'   : ['-Oz'],
      'cxxflags' : ['-Oz'],
   }
}

msvc_comp_args = {
   'debug': {
      'defines'  : default_windows_defines + ['-DDEBUG'],
      'warnings' : ['-W1'] + default_warning,
      'cflags'   : ['-MDd', '-Zi'],
      'cxxflags' : ['-MDd', '-Zi', '-EHsc', '-GR', '-std:c++14'],
      'ldflags'  : ['/debug']
   },
   'debugoptimized': {
      'defines'  : default_windows_defines + ['-DDEBUG'],
      'warnings' : ['-W1'] + default_warning,
      'cflags'   : ['-MD', '-Zi', '-O2'],
      'cxxflags' : ['-MD', '-Zi', '-EHsc', '-GR', '-std:c++14'],
      'ldflags'  : []
   },
   'release': {
      'defines'  : default_windows_defines + ['-DNDEBUG'],
      'warnings' : ['-W1'] + default_warning,
      'cflags'   : ['-MD', '-O2'],
      'cxxflags' : ['-MD', '-O2', '-EHsc', '-GR', '-std:c++14'],
      'ldflags'  : []
   },
   'minsize': {
      'defines'  : default_windows_defines + ['-DNDEBUG'],
      'warnings' : ['-W1'] + default_warning,
      'cflags'   : ['-MD', '-Os'],
      'cxxflags' : ['-MD', '-Os', '-EHsc', '-GR', '-std:c++14'],
      'ldflags'  : []
   }
}

#---------------------------------------------------------------------------------------------------

class Compiler:
   def __init__(self, name):
      self.name     = name
      self.defines  = []
      self.warnings = ['-Wall'] + default_warning
      self.cflags   = ['-pipe']
      self.cxxflags = self.cflags + ['-std=c++14']
      self.ldflags  = []


class ClangCompiler(Compiler):
   def __init__(self, buildtype, platform):
      super().__init__('clang')

      self.defines  = gnu_comp_args[buildtype]['defines'] 
      self.cflags   += gnu_comp_args[buildtype]['cflags'] + ['-fcolor-diagnostics']
      self.cxxflags += gnu_comp_args[buildtype]['cxxflags'] + ['-fcolor-diagnostics']

      if platform.is_windows():
         self.defines += default_windows_defines

      if platform.is_mingw():
         self.cxxflags += ['-femulated-tls']


class ClangclCompiler(Compiler):
   def __init__(self, buildtype, platform):
      super().__init__('clang-cl')

      self.defines  = msvc_comp_args[buildtype]['defines']
      self.warnings = msvc_comp_args[buildtype]['warnings']
      self.cflags   = msvc_comp_args[buildtype]['cflags']
      self.cxxflags = msvc_comp_args[buildtype]['cxxflags']
      self.ldflags  = msvc_comp_args[buildtype]['ldflags']


def make_compiler(name, buildenv):
   if buildenv.platform.is_msvc():
      return ClangclCompiler(buildenv.buildtype, buildenv.platform)
   else: 
      return ClangCompiler(buildenv.buildtype, buildenv.platform)


#---------------------------------------------------------------------------------------------------
# Name Templates

msvc_name_tpls = {
   'shlib' : {
      'prefix'   : '',
      'suffix'   : 'dll',
      'fname0'   : '{0.name}.{0.suffix}',                # no version
      'fname1'   : '{0.name}-{0.soversion}.{0.suffix}',  # with soversion
      'import'   : '{0.name}.lib',
   },
   'stlib' : {
      'prefix'   : 'lib',
      'suffix'   : 'lib',
      'fname'    : '{0.prefix}{0.name}.{0.suffix}',
   },
   'exec'  : {
      'fname'    : '{0.name}.exe'
   },
}

mingw_name_tpls = {
   'shlib' : {
      'prefix'   : 'lib',
      'suffix'   : 'dll',
      'fname0'   : '{0.prefix}{0.name}.{0.suffix}',               # no version
      'fname1'   : '{0.prefix}{0.name}-{0.soversion}.{0.suffix}', # with soversion
      'import'   : 'lib{0.name}.dll.a',
   },
   'stlib' : {
      'prefix'   : 'lib',
      'suffix'   : 'a',
      'fname'    : '{0.prefix}{0.name}.{0.suffix}',
   },
   'exec'  : {
      'fname'    : '{0.name}.exe'
   },
}

darwin_name_tpls = {
   'shlib' : {
      'prefix'   : 'lib',
      'suffix'   : 'dylib',
      'fname0'   : '{0.prefix}{0.name}.{0.suffix}',               # no version
      'fname1'   : '{0.prefix}{0.name}.{0.soversion}.{0.suffix}', # with soversion
      'import'   : '',
   },
   'stlib' : {
      'prefix'   : 'lib',
      'suffix'   : 'a',
      'fname'    : '{0.prefix}{0.name}.{0.suffix}',
   },
   'exec'  : {
      'fname'     : '{0.name}'
   },
}

gnu_name_tpls = {
   'shlib' : {
      'prefix'   : 'lib',
      'suffix'   : 'so',
      'fname0'   : '{0.prefix}{0.name}.{0.suffix}',               # no version
      'fname1'   : '{0.prefix}{0.name}.{0.suffix}.{0.soversion}', # libfoo.so.X
      'fname2'   : '{0.prefix}{0.name}.{0.suffix}.{0.ltversion}', # libfoo.so.X[.Y[.Z]] 
      'import'   : '',
   },
   'stlib' : {
      'prefix'   : 'lib',
      'suffix'   : 'a',
      'fname'    : '{0.prefix}{0.name}.{0.suffix}',
   },
   'exec'  : {
      'fname'    : '{0.name}'
   },
}

#---------------------------------------------------------------------------------------------------

class Target:
   def __init__(self, name, build_env):
      self.name = name
      self.build_env = build_env
      self.install = False
   

class BuildTarget(Target):
   def __init__(self, name, settings, build_env):
      super().__init__(name, build_env)

      self._sysname = build_env.platform.sysname()
      self._settings = settings
      
   def __str__(self):
      return 'Target {}'.format(self.name)

   def tool(self):
      return self._settings['tool']

   def defines(self):
      env_defines = 'defines-{}'.format(self._sysname)
      return self._settings.get('defines', []) + self._settings.get(env_defines, [])

   def includes(self):
      env_includes = 'includes-{}'.format(self._sysname)
      return self._settings.get('includes', []) + self._settings.get(env_includes, [])

   def cppflags(self):
      env_cppflags = 'cppflags-{}'.format(self._sysname)
      return self._settings.get('cppflags', [])  + self._settings.get(env_cppflags, [])

   def cflags(self):
      env_cflags = 'cflags-{}'.format(self._sysname)
      return self._settings.get('cflags', [])  + self._settings.get(env_cflags, [])

   def cxxflags(self):
      env_cxxflags = 'cxxflags-{}'.format(self._sysname)
      return self._settings.get('cxxflags', [])  + self._settings.get(env_cxxflags, [])

   def ldflags(self):
      env_ldflags = 'ldflags-{}'.format(self._sysname)
      return self._settings.get('ldflags', []) + self._settings.get(env_ldflags, [])

   def sources(self):
      env_sources = 'sources-{}'.format(self._sysname)
      return self._settings['sources'] + self._settings.get(env_sources, [])

   def objects(self):
      files = self.sources()
      return [self.build_env.src_to_obj(src_file) for src_file in files]

   def libs(self):
      platform_libs = []
      env_libs = []

      if self.build_env.platform.is_windows():
         platform_libs = windows_libs
         env_libs = self._settings.get('libs-{}'.format('windows'), [])

         if self.build_env.platform.is_mingw():
            platform_libs = mingw_windows_libs
            env_libs += self._settings.get('libs-{}'.format('mingw'), [])
      else:
         env_libs += self._settings.get('libs-{}'.format(self._sysname), [])

      return self._settings.get('libs', []) + platform_libs + env_libs  
      


class StaticLibrary(BuildTarget):
   def __init__(self, name, settings, build_env):
      super().__init__(name, settings, build_env)

      # Template names
      if self.build_env.platform.is_msvc():
         self.filename_tpl = msvc_name_tpls['stlib']
      elif self.build_env.platform.is_mingw():
         self.filename_tpl = mingw_name_tpls['stlib']
      elif self.build_env.platform.is_darwin():
         self.filename_tpl = darwin_name_tpls['stlib']
      else:
         self.filename_tpl = gnu_name_tpls['stlib']

      self.prefix = self.filename_tpl['prefix']
      self.suffix = self.filename_tpl['suffix']

   def __str__(self):
      return 'Static library {}'.format(self.name)

   def filename(self):
      return self.filename_tpl.get('fname', '').format(self)


class SharedLibrary(BuildTarget):
   def __init__(self, name, settings, build_env):
      super().__init__(name, settings, build_env)
      self.soversion = None
      self.ltversion = None

      # Template names
      if self.build_env.platform.is_msvc():
         self.filename_tpl = msvc_name_tpls['shlib']
      elif self.build_env.platform.is_mingw():
         self.filename_tpl = mingw_name_tpls['shlib']
      elif self.build_env.platform.is_darwin():
         self.filename_tpl = darwin_name_tpls['shlib']
      else:
         self.filename_tpl = gnu_name_tpls['shlib']

      self.prefix = self.filename_tpl['prefix']
      self.suffix = self.filename_tpl['suffix']

      # Shared library version
      if 'version' in settings:
         self.ltversion = settings['version']
         if not isinstance(self.ltversion, str):
            raise InvalidArguments('Shared library version needs to be a string, not ' + type(self.ltversion).__name__)
         if not re.fullmatch(r'[0-9]+(\.[0-9]+){0,2}', self.ltversion):
            raise InvalidArguments('Invalid Shared library version "{0}". Must be of the form X.Y.Z where all three are numbers. Y and Z are optional.'.format(self.ltversion))
      # Try to extract/deduce the soversion
      if 'soversion' in settings:
         self.soversion = settings['soversion']
         if isinstance(self.soversion, int):
            self.soversion = str(self.soversion)
         if not isinstance(self.soversion, str):
            raise InvalidArguments('Shared library soversion is not a string or integer.')
      elif self.ltversion:
         # library version is defined, get the soversion from that
         # We replicate what Autotools does here and take the first
         # number of the version by default.
         self.soversion = self.ltversion.split('.')[0]

   def __str__(self):
      return 'Shared library {}'.format(self.name)

   def filename(self):
      if self.build_env.platform.is_linux() and self.ltversion:
         return self.filename_tpl.get('fname2', '').format(self)
      elif self.soversion:
         return self.filename_tpl.get('fname1', '').format(self)

      return self.filename_tpl.get('fname0', '').format(self)

   def implib(self):
      return self.filename_tpl.get('import', '').format(self)
      
      
class Executable(BuildTarget):
   def __init__(self, name, settings, build_env):
      super().__init__(name, settings, build_env)
      self.suffix = None

      # Template names
      if self.build_env.platform.is_msvc():
         self.filename_tpl  = msvc_name_tpls['exec']
      elif self.build_env.platform.is_mingw():
         self.filename_tpl  = mingw_name_tpls['exec']
      elif self.build_env.platform.is_darwin():
         self.filename_tpl  = darwin_name_tpls['exec']
      else:
         self.filename_tpl  = gnu_name_tpls['exec']

   def __str__(self):
      return 'Executable {}'.format(self.name)

   def filename(self):
      return self.filename_tpl.get('fname', '').format(self)


#---------------------------------------------------------------------------------------------------

class BuildEnv:
   def __init__(self, options, args, platform):
      self.targets = {}
      self.buildtype = options.buildtype
      self.platform  = platform
      if options.host:
         self.host = Platform(options.host)
      else:
         self.host = self.platform

      self.build_dir = os.path.join(root_dir, args[0] if args[0] else 'build')
      if not os.path.isdir(self.build_dir):
         os.makedirs(self.build_dir)

      self.bin_dir = os.path.join(self.build_dir, BIN_OUT_NAME)
      self.lib_dir = os.path.join(self.build_dir, LIB_OUT_NAME)

      self.CC  = os.environ.get('CC',  'clang')
      self.CXX = os.environ.get('CXX', 'clang++')
      self.LD  = self.CXX
      self.AR  = os.environ.get('AR', 'ar')
      self.PYTHON = 'python3'

      self.compiler = make_compiler('clang', self)

   def __str__(self):
      return '''
      Build enviroment

      Platform   : {}
      Host       : {}
      Build type : {}
      Compiler   : {}

      Directories
      Root       : {}
      Build      : {}
      Bin        : {}
      Lib        : {}
      '''.format(self.platform.platform(), 
                 self.host.platform(), 
                 self.buildtype, 
                 self.compiler.name,
                 root_dir,
                 self.build_dir,
                 self.bin_dir,
                 self.lib_dir)

   def is_debug_build(self):
      return self.buildtype == 'debug'

   def default_defines(self):
      return self.compiler.defines
   
   def default_cflags(self):
      return list(set().union(CFLAGS, self.compiler.warnings, self.compiler.cflags))
   
   def default_cxxflags(self):
      return list(set().union(CXXFLAGS, self.compiler.warnings, self.compiler.cxxflags))

   def default_ldflags(self):
      return list(set().union(LDFLAGS, self.compiler.ldflags))

   def format_obj_name(self, name):
      return 'lib{}.o'.format(name)

   def src_to_obj(self, filename):
      obj_file = self.format_obj_name(os.path.splitext(filename)[0])
      obj_file = obj_file.replace('/', '_').replace('\\', '_')
      return '{}'.format(os.path.join(OBJ_OUT_NAME, obj_file))

   def lib_to_native(self, libname):
      # ex. 'boost_program_options' -> 'boost_program_options-vc140-mt-gd'
      if libname.startswith('boost'):
         if self.platform.is_msvc():
            return '{}-vc140-mt{}'.format(libname, '-gd' if self.is_debug_build() else '')
         else:
            #return '{}-mt{}'.format(libname, '-gd' if self.is_debug_build() else '')
            return '{}-mt'.format(libname)
      return libname

   def out_stlib(self, target):
      return os.path.join(LIB_OUT_NAME, target.filename())

   def out_shlib(self, target):
      return os.path.join(BIN_OUT_NAME, target.filename())

   def out_implib(self, target):
      return os.path.join(LIB_OUT_NAME, target.implib())

   def out_exec(self, target):
      return os.path.join(BIN_OUT_NAME, target.filename())

   def out_dep(self, target):
      if isinstance(target, StaticLibrary):
         return self.out_stlib(target)
      elif isinstance(target, SharedLibrary):
         return self.out_shlib(target)
      return ''

   def project_dep(self, libname):
      t = self.targets[libname]
      if isinstance(t, StaticLibrary):
         return self.out_stlib(t)
      return self.out_implib(t)

   def build_flags(self, target):
      includes = ['-I' + rel_to_dir(self.build_dir, dirname) for dirname in target.includes()]
   
      flags = {
         'defines'  : '$gbl_defines '  + ' '.join(target.defines()), 
         'includes' : '$gbl_includes ' + ' '.join(includes),
         'cppflags' : '$gbl_cppflags ' + ' '.join(target.cppflags()),
      }
   
      if target.tool() == 'cc':
         flags['cflags'] = '$gbl_cflags '   + ' '.join(target.cflags())
   
      if target.tool() == 'cxx':
         flags['cxxflags'] = '$gbl_cxxflags ' + ' '.join(target.cxxflags())
   
      return flags

   def shlib_ldflags(self, target):
      def fmt_lib(library):
         return '-l{}'.format(self.lib_to_native(library))
   
      args = {}
   
      implib = self.out_implib(target)
   
      args['ldflags'] = ['-shared'] + target.ldflags()
      args['libs']    = [fmt_lib(l) for l in target.libs()]
      return args
   
   def exec_ldflags(self, target):
      def fmt_lib(library):
         return '-l{}'.format(self.lib_to_native(library))
   
      args = {}
      args['ldflags'] = target.ldflags()
      args['libs']    = [fmt_lib(l) for l in target.libs()]
      return args

   def dependencies(self, target):
      deps = []
      for libname in target.libs():
         if libname in self.targets:
            deps += [self.out_dep(self.targets[libname])]
   
      return deps


class DarwinBuildEnv(BuildEnv):
   def __init__(self, options, args, platform):
      super().__init__(options, args, platform)

   def default_ldflags(self):
      return list(set().union(LDFLAGS, 
                              ['-dead_strip_dylibs', '-headerpad_max_install_names'],
                              self.compiler.ldflags))

   def project_dep(self, libname):
      t = self.targets[libname]
      if isinstance(t, StaticLibrary):
         return self.out_stlib(t)
      return self.out_shlib(t)

   def shlib_ldflags(self, target):
      def fmt_lib(library):
         if library in self.targets:
            return self.project_dep(library)
         return '-l{}'.format(self.lib_to_native(library))
   
      args = {}
   
      implib = self.out_implib(target)
      
      args['ldflags'] = ['-shared',
                         '-install_name @rpath/{}'.format(target.filename()),
                         '-compatibility_version 0',
                         '-current_version 0',
                         '-rpath {}'.format(self.lib_dir),
                         '-rpath {}'.format(self.bin_dir)]
      args['ldflags'].extend(target.ldflags())

      args['libs'] = [fmt_lib(l) for l in target.libs()]
      return args

   def exec_ldflags(self, target):
      def fmt_lib(library):
         if library in self.targets:
            return self.project_dep(library)
         return '-l{}'.format(self.lib_to_native(library))
   
      args = {}
   
      args['ldflags'] = ['-rpath {}'.format(self.lib_dir),
                         '-rpath {}'.format(self.bin_dir)] 
      args['ldflags'].extend(target.ldflags())

      args['libs'] = [fmt_lib(l) for l in target.libs()]
      return args
      

class MingwBuildEnv(BuildEnv):
   def __init__(self, options, args, platform):
      super().__init__(options, args, platform)

   def shlib_ldflags(self, target):
      def fmt_lib(library):
         if library in self.targets:
            return self.project_dep(library)
         return '-l{}'.format(self.lib_to_native(library))

      args = {}
   
      implib = self.out_implib(target)
      # Add linker flags
      args['ldflags'] = ['-shared', 
                         '-Wl,--no-undefined', 
                         '-Wl,--as-needed',
                         '-Wl,--out-implib={}'.format(implib)] + target.ldflags()
      # Add libs
      args['libs'] = [fmt_lib(l) for l in target.libs()]
      return args


   def exec_ldflags(self, target):
      def fmt_lib(library):
         if library in self.targets:
            return self.project_dep(library)
         return '-l{}'.format(self.lib_to_native(library))
   
      args = {}
   
      # Add linker flags
      args['ldflags'] = ['-Wl,--no-undefined', '-Wl,--as-needed'] + target.ldflags()
      # Add libs
      args['libs'] = [fmt_lib(l) for l in target.libs()]
      return args


class MsvcBuildEnv(BuildEnv):
   def __init__(self, options, args, platform):
      super().__init__(options, args, platform)

      self.CC  = os.environ.get('CC',  'clang-cl.exe')
      self.CXX = os.environ.get('CXX', 'clang-cl.exe')
      self.LD  = os.environ.get('LD',  'lld-link.exe')
      self.AR  = os.environ.get('AR',  'llvm-lib.exe')
      self.PYTHON = 'py.exe'

      self.compiler = make_compiler('clang-cl', self)

   def default_ldflags(self):
      return list(set().union(LDFLAGS, self.compiler.ldflags))

   def format_obj_name(self, name):
      return '{}.obj'.format(name)

   def shlib_ldflags(self, target):
      def fmt_lib(library):
         if library in self.targets:
            return self.project_dep(library)
         return '{}.lib'.format(self.lib_to_native(library))

      args = {}
   
      implib = self.out_implib(target)
      # Add linker flags
      args['ldflags'] = ['/dll', 
                         '/debug' if self.is_debug_build() else '',
                         '/implib:{}'.format(implib)] + \
                         target.ldflags()
      # Add libs
      args['libs'] = [fmt_lib(l) for l in target.libs()]
      return args

   def exec_ldflags(self, target):
      def fmt_lib(library):
         if library in self.targets:
            return self.project_dep(library)
         return '{}.lib'.format(library)
   
      args = {}
   
      # Add linker flags
      args['ldflags'] = ['/debug'] if self.is_debug_build() else []
      args['ldflags'].extend(target.ldflags())
      # Add libs
      args['libs'] = [fmt_lib(l) for l in target.libs()]
      return args
      
      
def make_buildenv(options, args):
   platform = Platform(options.platform)
   if platform.is_darwin():
      return DarwinBuildEnv(options, args, platform)
   elif platform.is_msvc():
      return MsvcBuildEnv(options, args, platform)
   elif platform.is_mingw():
      return MingwBuildEnv(options, args, platform)
   else: 
      return BuildEnv(options, args, platform)


#---------------------------------------------------------------------------------------------------

def rel_to_dir(dir, src_file):
   return os.path.relpath(os.path.join(root_dir, src_file), dir)

#---------------------------------------------------------------------------------------------------

def main(argv):
   parser = OptionParser()

   parser.add_option('-v', '--verbose', action='store_true',
                     help='enable verbose build')
   parser.add_option('--platform',
                     help='target platform ({})'.format('/'.join(Platform.known_platforms())),
                     choices=Platform.known_platforms())
   parser.add_option('--host',
                     help='host platform ({})'.format('/'.join(Platform.known_platforms())),
                     choices=Platform.known_platforms())
   parser.add_option('--buildtype', 
                     help='Build type to use (default: debug) ' + '/'.join(buildtypes),
                     choices=['debug', 'debugoptimized', 'release', 'minsize'],
                     default='debug')

   (options, args) = parser.parse_args()
   if len(args) != 1:
      parser.error("Incorrect number of arguments, a build directory is needed!")

   build_env = make_buildenv(options, args)

   print(build_env)

   static_libs = {}
   shared_libs = {}
   executables = {}

   declare_build_targets(build_env.platform, static_libs, shared_libs, executables)

   for name, settings in static_libs.items():
      build_env.targets[name] = StaticLibrary(name, settings, build_env)

   for name, settings in shared_libs.items():
      build_env.targets[name] = SharedLibrary(name, settings, build_env)

   for name, settings in executables.items():
      build_env.targets[name] = Executable(name, settings, build_env)

   write_ninja_file(build_env)

   return 0

def write_ninja_file(build_env):
   platform = build_env.platform
   targets = build_env.targets

   def rel_to_builddir(src_file):
      return os.path.relpath(os.path.join(root_dir, src_file), build_env.build_dir)

   n = ninja_syntax.Writer(open(os.path.join(build_env.build_dir, BUILD_FILENAME), 'w'), width=1000)

   write_ninja_header(n, build_env)
    
   n.comment('------------------------------------------------------------------------------')
   n.comment('Variables')
   n.comment('------------------------------------------------------------------------------')
   
   n.variable('gbl_defines', build_env.default_defines())
   n.variable('gbl_includes', '')
   n.variable('gbl_cppflags', CPPFLAGS)
   n.variable('gbl_cflags', build_env.default_cflags())
   n.variable('gbl_cxxflags', build_env.default_cxxflags())
   n.newline()

   n.variable('gbl_libs', LIBFLAGS)
   n.variable('gbl_ldflags', build_env.default_ldflags())
   n.newline()

   n.variable('cc', build_env.CC)
   n.variable('cxx', build_env.CXX)
   n.variable('ar', build_env.AR)
   n.variable('ld', build_env.LD)
   n.variable('python', build_env.PYTHON)
   n.newline()

   write_ninja_rules(n, build_env)

   all_targets = []

   for name, target in targets.items():
      n.comment('------------------------------------------------------------------------------')
      n.comment('Building {}'.format(target))
      n.comment('------------------------------------------------------------------------------')

      for src_file in target.sources():
         flags = build_env.build_flags(target)
         n.build(build_env.src_to_obj(src_file), target.tool(), rel_to_builddir(src_file), variables=flags)
         n.newline()

      if isinstance(target, StaticLibrary):
         all_targets += n.build(build_env.out_stlib(target), 'ar', target.objects())
      elif isinstance(target, SharedLibrary):
         all_targets += n.build(build_env.out_shlib(target), 
                                'link', 
                                target.objects(),
                                implicit=build_env.dependencies(target),
                                variables=build_env.shlib_ldflags(target))
      elif isinstance(target, Executable):
         all_targets += n.build(build_env.out_exec(target), 
                                'link', 
                                target.objects(),
                                implicit=build_env.dependencies(target),
                                variables=build_env.exec_ldflags(target))

      n.newline()

   n.comment('------------------------------------------------------------------------------')
   n.comment('Clean all files')
   n.comment('------------------------------------------------------------------------------')
   n.build('clean', 'CUSTOM_COMMAND', variables={'cmd'  : 'ninja -t clean', 
                                                 'desc' : 'Cleaning....'})
   n.newline()

   n.comment('------------------------------------------------------------------------------')
   n.comment('Print all primary targets')
   n.comment('------------------------------------------------------------------------------')
   n.build('targets', 'CUSTOM_COMMAND', variables={'cmd'  : 'ninja -t targets depth 2', 
                                                   'desc' : 'All primary targets:'})
   n.newline()
   
   n.comment('------------------------------------------------------------------------------')
   n.comment('Build All')
   n.comment('------------------------------------------------------------------------------')
   n.build('all', 'phony', all_targets)
   n.newline()

   n.default('all')
   n.newline()

   n.comment('------------------------------------------------------------------------------')
   n.comment('Regenerate configure.py')
   n.comment('------------------------------------------------------------------------------')
   n.build('build.ninja', 'REGENERATE_BUILD', '../configure.py', variables={'pool': 'console'})

   n.close()


def write_ninja_header(n, build_env):
   n.comment('------------------------------------------------------------------------------')
   n.comment('Project build file.')
   n.comment('It is generated by ' + os.path.basename(__file__) + '.')
   n.comment('------------------------------------------------------------------------------')
   n.newline()

   n.comment('------------------------------------------------------------------------------')
   n.comment('Minimal version of Ninja required by this file')
   n.comment('------------------------------------------------------------------------------')
   n.variable('ninja_required_version', '1.6')
   n.newline()

   n.comment('------------------------------------------------------------------------------')
   n.comment('Configuration: {}'.format(build_env.buildtype))
   n.comment('Platform:      {}'.format(build_env.platform.platform()))
   n.comment('------------------------------------------------------------------------------')
   n.newline()


def write_ninja_rules(n, build_env):
   platform = build_env.platform

   n.comment('------------------------------------------------------------------------------')
   n.comment('Core rules')
   n.comment('------------------------------------------------------------------------------')
   n.newline()

   if platform.is_msvc():
      n.rule('cc',
             command='$cc /showIncludes $defines $includes $cflags -c -o $out $in',
             deps='msvc',
             description='CC $out')
      n.newline()
      n.rule('cxx',
             command='$cxx /showIncludes $defines $includes $cxxflags -c -o $out $in',
             deps='msvc',
             description='CXX $out')
      n.newline()
      n.rule('ar',
             command='$ar $in /nologo -OUT:$out',
             description='AR $out')
      n.newline()
      n.rule('link',
             command='$ld /out:$out $ldflags $libs $in',
             description='LINK $out')
      n.newline()
   elif platform.is_darwin():
      n.rule('cc',
             command='$cc -MMD -MF $out.d $defines $includes $cflags -c $in -o $out',
             depfile='$out.d',
             deps='gcc',
             description='CC $out')
      n.newline()
      n.rule('cxx',
             command='$cxx -MMD -MF $out.d $defines $includes $cxxflags -c $in -o $out',
             depfile='$out.d',
             deps='gcc',
             description='CXX $out')
      n.newline()
      n.rule('ar',
             command='rm -f $out && libtool -static -o $out $in',
             description='AR $out')
      n.newline()
      n.rule('link',
             command='$ld $ldflags -o $out $in $solibs $libs',
             description='LINK $out')
      n.newline()
   else:
      n.rule('cc',
             command='$cc -MMD -MF $out.d $defines $includes $cflags -c $in -o $out',
             depfile='$out.d',
             deps='gcc',
             description='CC $out')
      n.newline()
      n.rule('cxx',
             command='$cxx -MMD -MF $out.d $defines $includes $cxxflags -c $in -o $out',
             depfile='$out.d',
             deps='gcc',
             description='CXX $out')
      n.newline()
      n.rule('ar',
             command='$ar rcsT $out $in',
             description='AR $out')
      n.rule('link',
             command='$ld $ldflags -o $out -Wl,--start-group $in $libs -Wl,--end-group $solibs',
             description='LINK $out')
      n.newline()

   n.comment('------------------------------------------------------------------------------')
   n.comment('Other rules')
   n.comment('------------------------------------------------------------------------------')
   n.newline()

   n.rule('CUSTOM_COMMAND',
           command='$cmd',
           description='$desc',
           restat=True)
   n.newline()
   n.rule('REGENERATE_BUILD',
           command='$python {} build'.format(os.path.join('..', 'configure.py')),
           description='Regenerating build files.',
           generator=1)
   n.newline()


#---------------------------------------------------------------------------------------------------

def declare_build_targets(platform, static_libraries, shared_libraries, executables):
   asio_defines = ['-DASIO_STANDALONE', '-DASIO_NO_DEPRECATED', '-DASIO_HAS_MOVE']
   boost_defines = ['-DBOOST_LIB_DIAGNOSTIC', '-DBOOST_ALL_NO_LIB', '-DBOOST_ALL_DYN_LINK']

   static_libraries['http-parser'] = {
      'tool'    : 'cc', 
      'includes': [],
      'sources' : [
         'deps/http-parser/http_parser.c'
      ]
   }

   shared_libraries['orion'] = {
      'tool'      : 'cxx',
      'cxxflags'  : [],
      'includes'  : ['include', 'lib', 'deps'],
      'defines'   : asio_defines + ['-DORION_SHARED_EXPORTS'],
      'soversion' : '0',
      'sources'   : [
         'lib/AsyncService.cpp',
         'lib/Encoding.cpp',
         'lib/System.cpp',
         # debug files
         'lib/debug/Stacktrace.cpp',
         # Logger files
         'lib/log/ExceptionRecord.cpp', 
         'lib/log/Formatter.cpp',   
         'lib/log/Function.cpp', 
         'lib/log/Level.cpp', 
         'lib/log/Logger.cpp', 
         'lib/log/LoggerImpl.cpp',  
         'lib/log/OutputHandler.cpp', 
         'lib/log/Record.cpp', 
         # Unit test files
         'lib/unittest/Test.cpp', 
         'lib/unittest/TestUtils.cpp', 
         'lib/unittest/TestResult.cpp', 
         'lib/unittest/TestRunner.cpp', 
         'lib/unittest/TestStdOutput.cpp', 
         'lib/unittest/TestSuite.cpp'
      ],
      'sources-darwin' : [
      #   'lib/debug/ModuleCache.cpp',
         'lib/debug/Stacktrace-darwin.cpp',
         'lib/debug/StacktraceFrame-posix.cpp',
      #   'lib/debug/Symbols.cpp',
      #   'lib/debug/dw/DwarfWrapper.cpp',
         'lib/Module-darwin.cpp',
         'lib/System-darwin.cpp',
         'lib/Uuid-darwin.cpp'
      ],
      'sources-linux' : [
         'lib/Module-linux.cpp',
         'lib/System-linux.cpp',
         'lib/Uuid-linux.cpp'
      ],
      'sources-mingw' : [
         'lib/debug/ModuleCache.cpp',
         'lib/debug/Stacktrace-win32.cpp',
         'lib/debug/StacktraceFrame-posix.cpp',
         'lib/debug/Symbols.cpp',
         'lib/debug/dw/COFF.cpp',
         'lib/debug/dw/DwarfWrapper.cpp',
         'lib/host/win32/Debug.cpp',
         'lib/io/MapFile-win32.cpp',
         'lib/Module-win32.cpp',
         'lib/System-win32.cpp',
         'lib/Uuid-win32.cpp'
      ],
      'sources-windows' : [
         'lib/debug/Stacktrace-win32.cpp',
         'lib/debug/StacktraceFrame-win32.cpp',
         'lib/host/win32/Debug.cpp',
         'lib/io/MapFile-win32.cpp',
         'lib/Module-win32.cpp',
         'lib/System-win32.cpp',
         'lib/Uuid-win32.cpp'
      ],
      'libs': [
        'fmt',
        'boost_program_options' # 'boost_program_options-vc140-mt-gd'
      ],
      'libs-mingw' : [
         'dwarf',
         'z'
      ],
      'libs-windows': [
         'dbghelp',
         'dbgeng',
         'Imagehlp'
      ]
   }

   shared_libraries['orion-net'] = {
      'tool'      : 'cxx',
      'cxxflags'  : [],
      'includes'  : ['include', 'lib', 'deps'],
      'defines'   : asio_defines + ['-DORION_SHARED_EXPORTS'],
      'soversion' : '0',
      'sources'   : [
         'lib/net/Address.cpp',
         'lib/net/AddressV4.cpp',
         'lib/net/AddressV6.cpp',
         'lib/net/EndPoint.cpp',
         'lib/net/Error.cpp',
         'lib/net/Url.cpp',
         # HTTP files
         'lib/net/http/Error.cpp',
         'lib/net/http/Parser.cpp',
         'lib/net/http/Request.cpp',
         'lib/net/http/RequestMux.cpp',
         'lib/net/http/Response.cpp',
         'lib/net/http/Server.cpp',
         'lib/net/http/ServerImpl.cpp',
         'lib/net/http/ServerConnection.cpp',
         'lib/net/http/Session.cpp',
         # TCP files
         'lib/net/tcp/Session.cpp',
         'lib/net/tcp/SessionImpl.cpp',
         'lib/net/tcp/Utils.cpp',
         # RPC files
         'lib/net/rpc/Error.cpp'
      ],
      'sources-darwin' : [
         'lib/net/AddressV4-darwin.cpp',
         'lib/net/AddressV6-darwin.cpp'
      ],
      'sources-mingw' : [
         'lib/net/AddressV4-win32.cpp',
         'lib/net/AddressV6-win32.cpp'
      ],
      'sources-windows' : [
         'lib/net/AddressV4-win32.cpp',
         'lib/net/AddressV6-win32.cpp'
      ],
      'libs': [
        'orion',
        'http-parser',
        'fmt'
      ],
   }

   #------------------------------------------------------------------------------------------------
   # Tests
   # 
   
   # Test: orion
   # 
   executables['test-orion'] = {
      'tool'     : 'cxx',
      'includes' : ['include', 'lib', 'deps', 'tests'],
      'sources'  : [
         'tests/test-base.cpp',
         'tests/test-encoding.cpp',
         'tests/test-logger.cpp',
         'tests/test-semver.cpp',
         'tests/test-string.cpp',
         'tests/test-units.cpp',
         'tests/test-unittests.cpp',
         'tests/test-main.cpp'
      ],
      'libs': ['fmt', 'orion']
   }

   # Test: orion
   # 
   executables['test-orion-net'] = {
      'tool'     : 'cxx',
      'includes' : ['include', 'lib', 'deps', 'tests'],
      'sources'  : [
         'tests/test-net.cpp',
         'tests/test-url.cpp',
         'tests/test-main.cpp'
      ],
      'libs': ['fmt', 'orion', 'orion-net']
   }

   #------------------------------------------------------------------------------------------------
   # Examples
   # 

   # Example: date-example
   #
   executables['date-example'] = {
      'tool'     : 'cxx',
      'includes' : ['include', 'deps'],
      'sources'  : [
         'examples/date-example.cpp'
      ],
      'libs': ['fmt', 'orion']
   }

   # Example: datetime-example
   #
   executables['datetime-example'] = {
      'tool'     : 'cxx',
      'includes' : ['include', 'deps'],
      'sources'  : [
         'examples/datetime-example.cpp'
      ],
      'libs': ['fmt', 'orion']
   }

   # Example: log-example
   #
   executables['log-example'] = {
      'tool'     : 'cxx',
      'includes' : ['include', 'deps'],
      'sources'  : [
         'examples/log-example.cpp'
      ],
      'libs': ['fmt', 'orion']
   }

   # Example: signal-example
   #
   executables['signal-example'] = {
      'tool'     : 'cxx',
      'includes' : ['include', 'deps'],
      'sources'  : [
         'examples/signal-example.cpp'
      ],
      'libs': ['fmt', 'orion']
   }

   # Example: string-examples
   #
   executables['string-examples'] = {
      'tool'     : 'cxx',
      'includes' : ['include', 'deps'],
      'sources'  : [
         'examples/string-examples.cpp'
      ],
      'libs': ['fmt', 'orion']
   }

   # Example: system-info
   #
   executables['system-info'] = {
      'tool'     : 'cxx',
      'includes' : ['include', 'deps'],
      'sources'  : [
         'examples/system-info.cpp'
      ],
      'libs': ['fmt', 'orion']
   }

   # Example: hello-http-server
   #
   executables['hello-http-server'] = {
      'tool'     : 'cxx',
      'includes' : ['include', 'deps'],
      'defines'  : asio_defines,
      'sources'  : [
         'examples/hello-http-server.cpp'
      ],
      'libs': ['fmt', 'orion', 'orion-net']
   }

   # Example: hello-http-client
   #
   executables['hello-http-client'] = {
      'tool'     : 'cxx',
      'includes' : ['include', 'deps'],
      'defines'  : asio_defines,
      'sources'  : [
         'examples/hello-http-client.cpp'
      ],
      'libs': ['fmt', 'orion', 'orion-net']
   }

   # Example: hello-tcp-server
   #
   executables['hello-tcp-server'] = {
      'tool'     : 'cxx',
      'includes' : ['include', 'deps'],
      'defines'  : asio_defines,
      'sources'  : [
         'examples/hello-tcp-server.cpp'
      ],
      'libs': ['fmt', 'orion', 'orion-net']
   }

   # Example: hello-tcp-client
   #
   executables['hello-tcp-client'] = {
      'tool'     : 'cxx',
      'includes' : ['include', 'deps'],
      'defines'  : asio_defines,
      'sources'  : [
         'examples/hello-tcp-client.cpp'
      ],
      'libs': ['fmt', 'orion', 'orion-net']
   }

#---------------------------------------------------------------------------------------------------

if __name__ == '__main__':
  sys.exit(main(sys.argv[1:]))
