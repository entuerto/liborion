/*
 * Copyright (c) 2016 Tomas Palazuelos
 * Licensed under the MIT license.
 */
module.exports = function(grunt) {
	"use strict";

	grunt.AR   = ''; // Archive-maintaining program
	grunt.CC   = ''; // Program for compiling C programs
	grunt.CXX  = ''; // Program for compiling C++ programs
	grunt.LINK = '';

	grunt.defOptions = {
			'darwin'  : {
				cxxflags : ["-Wall"],
				cppflags : [],
				includes : ["."],
				ldflags  : [],
				libs     : [],
				outDir   : grunt.buildDir
			},
			'freebsd' : {
				cxxflags : ["-Wall"],
				cppflags : [],
				includes : ["."],
				ldflags  : [],
				libs     : [],
				outDir   : grunt.buildDir
			}, 
			'linux'   : {
				cxxflags : ["-Wall"],
				cppflags : [],
				includes : ["."],
				ldflags  : [],
				libs     : [],
				outDir   : grunt.buildDir
			}, 
			'win32'   : {
				cxxflags : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
				cppflags : ["-D __MINGW64__", "-D WIN32"],
				includes : ["."],
				ldflags  : [],
				libs     : [],
				outDir   : grunt.buildDir
			}
		};

	grunt.docsDir = 'docs';

	var path = require("path");

	grunt.file.findProgram = function(name) {
		var PATH = process.env.PATH.split(path.delimiter);

		PATH.forEach(function(dir) {
			var file = path.join(dir, name);
			if (grunt.file.exists(file)) {
				return true;
			}
		});
		return false;
	};
};
