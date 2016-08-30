/*
 * Copyright (c) 2016 Tomas Palazuelos
 * Licensed under the MIT license.
 */
module.exports = function(grunt) {
	"use strict";

	var path = require("path").posix;
	var util = require('util');

	var pkg = grunt.config.get('pkg');

	grunt.file.findProgram = function(name) {
		var PATH = process.env.PATH.split(path.delimiter);

		for (let dir of PATH) {
			var file = path.join(dir, name);

			if (grunt.file.exists(file)) {
				return file;
			}
		}

		//throw new Error("findProgram: program not found, " + name);
		return name;
	};

	grunt.build = {};
	grunt.build.opts = {};

	grunt.build.opts.common = {
		patterns: {
			define  : '-D %s',
			include : '-I %s',
			lib     : '-l%s',
			libpath : '-L%s',
			stlib   : 'lib%s.a',
			shlib   : 'lib%s-%s.so',
			implib  : '',
			prog    : '%s',
			objExt  : 'o',
			objGlob : '*.o',
			version : '%s.%s'
		},
		options: {
			debug   : {
				cc: {
					cflags   : ['-g'],
					cppflags : ['-D DEBUG']
				},
				cxx: {
					cxxflags : ['-g'],
					cppflags : ['-D DEBUG']
				},
				stlib: {},
				shlib: {
					stdLibs : []
				},
				program: {
					stdLibs : []
				}
			},
			release : {
				cc: {
					cflags   : ['-O'],
					cppflags : ['-D NDEBUG']
				},
				cxx: {
					cxxflags : ['-O'],
					cppflags : ['-D NDEBUG']
				},
				stlib: {},
				shlib: {
					stdLibs : []
				},
				program: {
					stdLibs : []
				}
			}
		},
		cc: {
			CC       : '',
			template : '${CC} ${CFLAGS} ${CPPFLAGS} ${DEFINES} ${INCLUDES} -c ${SRC} -o ${OUT}',
			options  :{
				defines : [],
				includes: ['.'],
				cflags  : ["-Wall"],
				cppflags: []
			}
		},
		cxx: {
			CXX      : '', 
			template : '${CXX} ${CXXFLAGS} ${CPPFLAGS} ${DEFINES} ${INCLUDES} -c ${SRC} -o ${OUT}',
			options  : {
				defines : [],
				includes: ['.'],
				cxxflags: ["-Wall", "-std=c++14"],
				cppflags: []
			}
		},
		shlib: {
			LINK     : '',
			template : '${LINK} -o ${OUT} ${OBJS} -link ${LDFLAGS} ${LDLIBS}',
			options  : {
				ldflags : [],
				libPaths: [],
				implib  : [],
				libs    : []
			}
		},
		stlib: {
			AR       : '', 
			template : '${AR} cr ${OUT} ${OBJS}',
			options  : {}
		},
		program: {
			LINK     : '',
			template : '${LINK} -o ${OUT} ${OBJS} -link ${LDFLAGS} ${LDLIBS}',
			options  : {
				ldflags : [],
				libPaths: [],
				libs    : []
			}
		}	
	};

	grunt.build.opts['darwin-clang'] = {
		patterns: {
			sharedLibPrefix: "lib",
			sharedLibSuffix: "dylib"
		}
	};

	grunt.build.opts['win32-g++'] = {
		patterns: {
			stlib   : 'lib%s.a',
			shlib   : 'lib%s-%s.dll',
			implib  : '--out-implib,%s/lib%s.dll.a',
			prog    : '%s.exe',
			objExt  : 'o',
			objGlob : '*.o'
		},
		options: {
			defines: ['WIN32', 'UNICODE'],
			stdLibs: ['-lkernel32', 
			          '-luser32', 
			          '-lgdi32', 
			          '-lwinspool', 
			          '-lshell32', 
			          '-lole32', 
			          '-loleaut32', 
			          '-luuid', 
			          '-lcomdlg32', 
			          '-ladvapi32']
		},
		cc: {
			CC     : grunt.file.findProgram('gcc.exe'),
			options: {
				defines : ['WIN32', 'UNICODE']
			}
		},
		cxx: {
			CXX    : grunt.file.findProgram('g++.exe'), 
			options: {
				defines : ['WIN32', 'UNICODE']
			}
		},
		stlib: {
			AR : grunt.file.findProgram('ar.exe'), 
		},
		shlib: {
			LINK     : grunt.file.findProgram('g++.exe'),
			template : '${LINK} -shared -o ${OUT} ${OBJS} -Wl,${IMPLIB} ${LDFLAGS} ${LDLIBS}',
			options  : {
				ldflags:  ['-Wl,-Bdynamic', '-Wl,--enable-auto-import'],
				libPaths: [],
				libs    : []
			}
		},
		program: {
			LINK     : grunt.file.findProgram('g++.exe'),
			template : '${LINK} -o ${OUT} ${OBJS}  ${LDFLAGS} ${LDLIBS}',
			options  : {
				ldflags : ['-Wl,-Bdynamic', '-Wl,--enable-auto-import'],
				libPaths: [],
				libs    : []
			}
		}
	};

	grunt.build.opts['win32-clang-cl'] = {
		patterns: {
			lib     : '%s.lib',
			libpath : '-libpath:%s',
			stlib   : 'lib%s.lib',
			shlib   : '%s-%s.dll',
			implib  : '-implib:%s/%s.lib',
			prog    : '%s.exe',
			objExt  : 'obj',
			objGlob : '*.obj'
		},
		options: {
			debug   : {
				cc: {
					cflags   : ['-MDd', '-Zi', '-Ob0', '-Od'],
					cppflags : ['-D DEBUG', '-D _DEBUG']
				},
				cxx: {
					cxxflags : ['-MDd', '-Zi', '-Ob0', '-Od'],
					cppflags : ['-D DEBUG', '-D _DEBUG']
				},
				stlib: {},
				shlib: {
					stdLibs : ['kernel32', 
				               'user32', 
				               'gdi32', 
				               'winspool', 
				               'shell32', 
				               'ole32', 
				               'oleaut32', 
				               'uuid', 
				               'comdlg32', 
				               'advapi32']
				},
				program: {
					stdLibs : ['kernel32', 
				               'user32', 
				               'gdi32', 
				               'winspool', 
				               'shell32', 
				               'ole32', 
				               'oleaut32', 
				               'uuid', 
				               'comdlg32', 
				               'advapi32']
				}
			},
			release : {
				cc: {
					cflags   : ['-MD', '-O2', '-Ob2'],
					cppflags : ['-D NDEBUG']
				},
				cxx: {
					cxxflags : ['-MD', '-O2', '-Ob2'],
					cppflags : ['-D NDEBUG']
				},
				stlib: {},
				shlib: {
					stdLibs : ['kernel32', 
				               'user32', 
				               'gdi32', 
				               'winspool', 
				               'shell32', 
				               'ole32', 
				               'oleaut32', 
				               'uuid', 
				               'comdlg32', 
				               'advapi32']
				},
				program: {
					stdLibs : ['kernel32', 
				               'user32', 
				               'gdi32', 
				               'winspool', 
				               'shell32', 
				               'ole32', 
				               'oleaut32', 
				               'uuid', 
				               'comdlg32', 
				               'advapi32']
				}
			}
		},
		cc: {
			CC       : grunt.file.findProgram('clang-cl.exe'),
			options  : {
				defines : ['WIN32', 'UNICODE'],
				cflags  : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
				cppflags: []
			}
		},
		cxx: {
			CXX      : grunt.file.findProgram('clang-cl.exe'), 
			options  : {
				defines : ['WIN32', 'UNICODE'],
				cxxflags: ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
				cppflags: []
			}
		},
		shlib: {
			LINK     : grunt.file.findProgram('clang-cl.exe'),
			template : '${LINK} -o ${OUT} ${OBJS} -link ${IMPLIB} ${LDFLAGS} ${LDLIBS}',
			options  : {
				ldflags : ["-dll", "-dynamicbase"],
				libs    : [],
				libPaths: [],
				implib  : [],
				libs    : []
			}
		},
		stlib: {
			AR       : grunt.file.findProgram('llvm-lib.exe'), 
			template : '${AR} -out:${OUT} ${OBJS}',
			options  : {}
		},
		program: {
			LINK     : grunt.file.findProgram('clang-cl.exe'),
			template : '${LINK} -o ${OUT} ${OBJS} -link ${LDFLAGS} ${LDLIBS}',
			options  : {
				ldflags : [],
				libs    : [],
				libPaths: [],
				libs    : []
			}
		}
	};

	grunt.docsDir  = 'docs';
	grunt.buildDir = 'build';

	var _  = require('lodash');
	var fs = require('fs');

	//
	// CXX       = Program for compiling C++
	// CXXFLAGS  = Extra flags to give to the C++ compiler.
	// CPPFLAGS  = Extra flags to give to the C preprocessor and programs that use it.
	// INCLUDES  = 
	// DEFINES   = 
	// SRC       = Source file to compile
	// OUT       = Object file produced
	//
	// ${CXX} ${CXXFLAGS} ${CPPFLAGS} ${INCLUDES} ${DEFINES} -c ${SRC} -o ${OUT}

	//
	// LINK      = Program for linking C++
	// LINKFLAGS
	// LIB
	// LDFLAGS   = Extra flags to give to compilers when they are supposed to invoke the linker
	// LDLIBS    = Library flags or names given to compilers when they are supposed to invoke the linker
	// OUT       = Shared library produced
	//
	// ${LINK} -o ${OUT} ${OBJS} -link -implib:${IMPLIB} ${LDFLAGS} ${LIB}

	function customizer(objValue, srcValue) {
		if (_.isArray(objValue)) {
			return _.uniq(objValue.concat(srcValue));
		}
	};

	function defaultCompiler() {
		switch(process.platform) {
			case 'darwin':
			case 'freebsd':
				return 'clang++'
			case 'linux':
			case 'sunos':
				return 'g++';
			case 'win32':
				return 'clang-cl'
			default:
				break;
		}
		throw "Unkown platform."
	};

	function defaultPrefix() {
		switch(process.platform) {
			case 'darwin':
			case 'freebsd':
			case 'linux':
			case 'sunos':
				return '/usr/local';
			case 'win32':
				return 'C:/Program Files'
			default:
				break;
		}
		throw "Unkown platform."
	};

	var buildConfig = null;

	grunt.build.config = function(name, userOptions) {
		if (!buildConfig) {
			let configFile = path.join(grunt.buildDir, 'config.json');

			if (!fs.existsSync(configFile))
				throw new Error("Configure file not found, " + configFile);

			buildConfig = grunt.file.readJSON(configFile);

			buildConfig.cc.cmd      = _.template(buildConfig.cc.template);
			buildConfig.cxx.cmd     = _.template(buildConfig.cxx.template);
			buildConfig.shlib.cmd   = _.template(buildConfig.shlib.template);
			buildConfig.stlib.cmd   = _.template(buildConfig.stlib.template);
			buildConfig.program.cmd = _.template(buildConfig.program.template);
		}
		
		let c = Object.assign({}, buildConfig[name]);

		c.options = _.mergeWith({}, c.options, userOptions, customizer);

		return c; 
	};

	grunt.build.patterns = function() {
		return buildConfig.patterns;
	};

	grunt.registerTask('configure', 'Configure build enviroment', function() {
		if (!fs.existsSync(grunt.buildDir))
			fs.mkdirSync(grunt.buildDir);

		let defaults = {},
		    outFile  = path.join(grunt.buildDir, 'config.json'),
			pkg      = grunt.config.get('pkg'),
		    compiler = grunt.option('compiler') || defaultCompiler(),
		    prefix   = path.join(grunt.option('prefix') || defaultPrefix(), pkg.name),
		    bindir   = grunt.option('bindir') || path.join(prefix, 'bin'),
		    datadir  = grunt.option('datadir') || path.join(prefix, 'share'),
		    libdir   = grunt.option('libdir')  || path.join(prefix, 'lib'),
		    includedir = grunt.option('includedir') || path.join(prefix, 'include');

		_.merge(defaults, grunt.build.opts.common, 
			              grunt.build.opts[util.format('%s-%s', process.platform, compiler)]);

		let opt = {};

		_.mergeWith(opt, defaults.options, 
			             defaults.options.debug, 
			             customizer);

		delete opt.debug;
		delete opt.release;

		let tgt = {
			patterns: defaults.patterns
		};

		tgt.cc = defaults.cc;
		_.mergeWith(tgt.cc.options, opt.cc, customizer);

		tgt.cxx = defaults.cxx;
		_.mergeWith(tgt.cxx.options, opt.cxx, customizer);

		tgt.stlib = defaults.stlib;
		_.mergeWith(tgt.stlib.options, opt.stlib, customizer);

		tgt.shlib = defaults.shlib;
		_.mergeWith(tgt.shlib.options, opt.shlib, customizer);

		tgt.program = defaults.program;
		_.mergeWith(tgt.program.options, opt.program, customizer);
	
		tgt.dirs = {
			prefix    : prefix,
			bindir    : bindir,
			datadir   : datadir,
			libdir    : libdir,
			includedir: includedir
		};

		grunt.log.writeln();
		grunt.log.writeln("liborion " + pkg.version);
		grunt.log.writeln();
		grunt.log.writeln("Configuration:");
		grunt.log.writeln()
		grunt.log.writeln("  prefix                  : " + prefix); 
		grunt.log.writeln("  bindir                  : " + bindir); 
		grunt.log.writeln("  datadir                 : " + datadir); 
		grunt.log.writeln("  libdir                  : " + libdir); 
		grunt.log.writeln("  includedir              : " + includedir); 
		grunt.log.writeln("  C Compiler              : " + tgt.cc.CC);
		grunt.log.writeln("  C++ Compiler            : " + tgt.cxx.CXX)
		grunt.log.writeln(util.format("  CFLAGS                  : %s", tgt.cc.options.cflags));
		grunt.log.writeln(util.format("  CXXFLAGS                : %s", tgt.cxx.options.cxxflags));
		grunt.log.writeln(util.format("  CPPFLAGS                : %s", tgt.cxx.options.cppflags));

		grunt.file.write(outFile, JSON.stringify(tgt, null, '\t'));
	});
};
