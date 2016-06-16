/*
 * Copyright (c) 2016 Tomas Palazuelos
 * Licensed under the MIT license.
 */
module.exports = function(grunt) {
	"use strict";

	var path = require("path");

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
			shlib   : 'lib%s.so',
			implib  : '',
			prog    : '%s',
			objExt  : 'o',
			objGlob : '*.o'
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
				cxxflags: ["-Wall"],
				cppflags: []
			}
		},
		shlib: {
			LINK     : '',
			template : '${LINK} -o ${OUT} ${OBJS} -link ${LDFLAGS} ${LDLIBS}',
			options  : {
				ldflags : [],
				libs    : [],
				libPaths: [],
				implib  : [],
				libs    : []
			}
		},
		stlib: {
			AR       : '', 
			template : '${AR} -out:${OUT} ${OBJS}',
			options  : {}
		},
		program: {
			LINK     : '',
			template : '${LINK} -o ${OUT} ${OBJS} -link ${LDFLAGS} ${LDLIBS}',
			options  : {
				ldflags : [],
				libs    : [],
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
			stlib   : 'lib%s.lib',
			shlib   : 'lib%s.dll',
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
		}
	};

	grunt.build.opts['win32-clang-cl'] = {
		patterns: {
			lib     : '%s',
			libpath : '-libpath:%s',
			stlib   : 'lib%s.lib',
			shlib   : 'lib%s.dll',
			implib  : '-implib:%s/lib%s.dll.lib',
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
					stdLibs : ['kernel32.lib', 
				               'user32.lib', 
				               'gdi32.lib', 
				               'winspool.lib', 
				               'shell32.lib', 
				               'ole32.lib', 
				               'oleaut32.lib', 
				               'uuid.lib', 
				               'comdlg32.lib', 
				               'advapi32.lib']
				},
				program: {
					stdLibs : ['kernel32.lib', 
				               'user32.lib', 
				               'gdi32.lib', 
				               'winspool.lib', 
				               'shell32.lib', 
				               'ole32.lib', 
				               'oleaut32.lib', 
				               'uuid.lib', 
				               'comdlg32.lib', 
				               'advapi32.lib']
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
					stdLibs : ['kernel32.lib', 
				               'user32.lib', 
				               'gdi32.lib', 
				               'winspool.lib', 
				               'shell32.lib', 
				               'ole32.lib', 
				               'oleaut32.lib', 
				               'uuid.lib', 
				               'comdlg32.lib', 
				               'advapi32.lib']
				},
				program: {
					stdLibs : ['kernel32.lib', 
				               'user32.lib', 
				               'gdi32.lib', 
				               'winspool.lib', 
				               'shell32.lib', 
				               'ole32.lib', 
				               'oleaut32.lib', 
				               'uuid.lib', 
				               'comdlg32.lib', 
				               'advapi32.lib']
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
		    outFile  = path.join(grunt.buildDir, 'config.json');

		_.merge(defaults, grunt.build.opts.common, 
			              grunt.build.opts['win32-clang-cl']);

		let opt = {};

		_.mergeWith(opt, defaults.options, 
			             defaults.options.release, 
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
	
		grunt.file.write(outFile, JSON.stringify(tgt, null, '\t'));

	});
};
