/*
 * Copyright (c) 2016 Tomas Palazuelos
 * Licensed under the MIT license.
 */
module.exports = function(grunt) {
	"use strict";

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
				programs: {
					AR   : 'llvm-lib.exe', // Archive-maintaining program
					CC   : 'clang-cl.exe', // Program for compiling C programs
					CXX  : 'clang-cl.exe', // Program for compiling C++ programs
					LINK : 'clang-cl.exe'
				},
				patterns: {
					define  : '-D %s',
					include : '-I %s',
					stlib   : 'lib%s.lib',
					shlib   : 'lib%s.dll',
					implib  : 'lib%s.dll.lib'
				},
				options: {
					cxxflags : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
					cppflags : [],
					includes : ["."],
					defines  : ["__MINGW64__", "WIN32"],
					ldflags  : [],
					libs     : [],
					outDir   : grunt.buildDir
				}
			}
		};

	grunt.docsDir = 'docs';

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

	var _ = require('lodash');

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

	grunt.cxxshlibConfig = function(userOptions) {
		let defaults = grunt.buildDefaults[process.platform];

		return {
			programs: {
				AR   : grunt.file.findProgram(defaults.programs.AR), 
				CC   : grunt.file.findProgram(defaults.programs.CC), 
				CXX  : grunt.file.findProgram(defaults.programs.CXX), 
				LINK : grunt.file.findProgram(defaults.programs.LINK)
			},
			patterns: _.mergeWith({}, defaults.patterns, customizer),
			templates: {
				cxx  : _.template('${CXX} ${CXXFLAGS} ${CPPFLAGS} ${DEFINES} ${INCLUDES} -c ${SRC} -o ${OUT}'),
				link : _.template('${LINK} -o ${OUT} ${OBJS} -link -implib:${IMPLIB} ${LDFLAGS} ${LDLIBS}')
			},
			options: _.mergeWith(defaults.options, userOptions, customizer)
		}
	};

	grunt.cxxstlibConfig = function(userOptions) {
		let defaults = grunt.buildDefaults[process.platform];

		return {
			programs: {
				AR   : grunt.file.findProgram(defaults.programs.AR), 
				CC   : grunt.file.findProgram(defaults.programs.CC), 
				CXX  : grunt.file.findProgram(defaults.programs.CXX), 
				LINK : grunt.file.findProgram(defaults.programs.LINK)
			},
			patterns: _.mergeWith({}, defaults.patterns, customizer),
			templates: {
				cxx : _.template('${CXX} ${CXXFLAGS} ${CPPFLAGS} ${DEFINES} ${INCLUDES} -c ${SRC} -o ${OUT}'),
				ar  : _.template('${AR} -out:${OUT} ${OBJS}')
			},
			options: _.mergeWith(defaults.options, userOptions, customizer)
		}
	};

	grunt.cxxprogramConfig = function(userOptions) {
		let defaults = grunt.buildDefaults[process.platform];

		return {
			programs: {
				AR   : grunt.file.findProgram(defaults.programs.AR), 
				CC   : grunt.file.findProgram(defaults.programs.CC), 
				CXX  : grunt.file.findProgram(defaults.programs.CXX), 
				LINK : grunt.file.findProgram(defaults.programs.LINK)
			},
			patterns: _.mergeWith({}, defaults.patterns, customizer),
			templates: {
				cxx  : _.template('${CXX} ${CXXFLAGS} ${CPPFLAGS} ${DEFINES} ${INCLUDES} -c ${SRC} -o ${OUT}'),
				link : _.template('${LINK} -o ${OUT} ${OBJS} -link ${LDFLAGS} ${LDLIBS}')
			},
			options: _.mergeWith(defaults.options, userOptions, customizer)
		}
	};
};
