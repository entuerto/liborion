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
		throw new Error("findProgram: program not found, " + name);
	};

	grunt.buildDefaults = {
			'darwin'  : {
				programs: {
					AR   : 'llvm-ar', // Archive-maintaining program
					CC   : 'clang',   // Program for compiling C programs
					CXX  : 'clang++', // Program for compiling C++ programs
					LINK : 'clang'
				},
				options: {
					cxxflags : ["-Wall"],
					cppflags : [],
					includes : ["."],
					defines  : [],
					ldflags  : [],
					libs     : [],
					outDir   : grunt.buildDir
				}
			},
			'freebsd' : {
				programs: {
					AR   : 'llvm-ar', // Archive-maintaining program
					CC   : 'clang',   // Program for compiling C programs
					CXX  : 'clang++', // Program for compiling C++ programs
					LINK : 'clang'
				},
				options: {
					cxxflags : ["-Wall"],
					cppflags : [],
					includes : ["."],
					defines  : [],
					ldflags  : [],
					libs     : [],
					outDir   : grunt.buildDir
				}
			}, 
			'linux'   : {
				programs: {
					AR   : 'llvm-ar', // Archive-maintaining program
					CC   : 'clang',   // Program for compiling C programs
					CXX  : 'clang++', // Program for compiling C++ programs
					LINK : 'clang'
				},
				options: {
					cxxflags : ["-Wall"],
					cppflags : [],
					includes : ["."],
					defines  : [],
					ldflags  : [],
					libs     : [],
					outDir   : grunt.buildDir
				}
			}, 
			'win32'   : {
				patterns: {
					define  : '-D %s',
					include : '-I %s',
					stlib   : 'lib%s.lib',
					shlib   : 'lib%s.dll',
					implib  : 'lib%s.dll.lib',
					prog    : '%s.exe',
					objExt  : 'obj',
					objGlob : '*.obj'
				},
				options: {
					includes : ["."],
					defines  : ["__MINGW64__", "WIN32"],
					outDir   : grunt.buildDir
				},
				cc: {
					CC       : grunt.file.findProgram('clang-cl.exe'),
					template : '${CC} ${CFLAGS} ${CPPFLAGS} ${DEFINES} ${INCLUDES} -c ${SRC} -o ${OUT}',
					options  :{
						cflags   : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
						cppflags : []
					}
				},
				cxx: {
					CXX      : grunt.file.findProgram('clang-cl.exe'), 
					template : '${CXX} ${CXXFLAGS} ${CPPFLAGS} ${DEFINES} ${INCLUDES} -c ${SRC} -o ${OUT}',
					options  : {
						cxxflags : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
						cppflags : []
					}
				},
				shlib: {
					LINK     : grunt.file.findProgram('clang-cl.exe'),
					template : '${LINK} -o ${OUT} ${OBJS} -link -implib:${IMPLIB} ${LDFLAGS} ${LDLIBS}',
					options  : {
						ldflags  : ["-dll", "-dynamicbase", '-libpath:build/'],
						libs     : []
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
						ldflags : ['-libpath:build/'],
						libs    : []
					}
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

	grunt.build = {};
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
		
		_.mergeWith(buildConfig[name].options, userOptions, customizer);

		return buildConfig[name]; 
	};

	grunt.build.patterns = function(name) {
		return grunt.buildDefaults[process.platform].patterns;
	};

	grunt.registerTask('configure', 'Configure build enviroment', function() {
		if (!fs.existsSync(grunt.buildDir))
			fs.mkdirSync(grunt.buildDir);

		let defaults = grunt.buildDefaults[process.platform],
		    outFile  = path.join(grunt.buildDir, 'config.json');

		_.mergeWith(defaults.cc.options, defaults.options, customizer);
		_.mergeWith(defaults.cxx.options, defaults.options, customizer);
		_.mergeWith(defaults.shlib.options, defaults.options, customizer);
		_.mergeWith(defaults.stlib.options, defaults.options, customizer);
		_.mergeWith(defaults.program.options, defaults.options, customizer);

		grunt.file.write(outFile, JSON.stringify(defaults, null, '\t'));
	});
};
