/*
 * Copyright (c) 2016 Tomas Palazuelos
 * Licensed under the MIT license.
 */
module.exports = function(grunt) {
	"use strict";

	var spawn = require('child_process').spawnSync;
	var fs   = require('fs');
	var path = require('path');
	var util = require('util');

	grunt.buildDir = 'build';

	if (!fs.existsSync(grunt.buildDir))
		fs.mkdirSync(grunt.buildDir);

	grunt.registerMultiTask('cxxshlib', 'Compile and link a shared library', function() {
		var options = this.options({
			cxxflags: [],
			cppflags: [],
			includes: [],
			ldflags : ["-dll", "-dynamicbase", '-libpath:build/'],
			libs    : [],
			outDir  : path.join(grunt.buildDir, this.target)
		});

		if (!fs.existsSync(options.outDir))
			fs.mkdirSync(options.outDir);

		grunt.config.set('cxx.' + this.target, options);
		grunt.config.set('shlib.' + this.target, options);

		var done = this.async();

		var target = this.target;
		// Iterate over all src files.
		var taskList = this.filesSrc.map(function(srcFile) {
			return 'cxx:' + target + ':' + srcFile;
		});

		grunt.task.run(taskList);
		grunt.task.run('shlib:' + target);

		done();
	});

	grunt.registerMultiTask('cxxstlib', 'Compile and create static library', function() {
		var options = this.options({
			cxxflags: [],
			cppflags: [],
			includes: [],
			libs    : [],
			outDir  : path.join(grunt.buildDir, this.target)
		});

		if (!fs.existsSync(options.outDir))
			fs.mkdirSync(options.outDir);

		grunt.config.set('cxx.' + this.target, options);
		grunt.config.set('stlib.' + this.target, options);

		var done = this.async();

		var target = this.target;
		// Iterate over all src files.
		var taskList = this.filesSrc.map(function(srcFile) {
			return 'cxx:' + target + ':' + srcFile;
		});

		grunt.task.run(taskList);
		grunt.task.run('stlib:' + target);

		done();
	});

	grunt.registerMultiTask('cxxprogram', 'Compile and link a program', function() {
		var options = this.options({
			cxxflags: [],
			cppflags: [],
			includes: [],
			ldflags : ['-libpath:build/'],
			libs    : [],
			outDir  : path.join(grunt.buildDir, this.target)
        });

		if (!fs.existsSync(options.outDir))
			fs.mkdirSync(options.outDir);

		grunt.config.set('cxx.' + this.target, options);
		grunt.config.set('program.' + this.target, options);

		var done = this.async();

		var target = this.target;
		// Iterate over all src files.
		var taskList = this.filesSrc.map(function(srcFile) {
			return 'cxx:' + target + ':' + srcFile;
		});

		grunt.task.run(taskList);
		grunt.task.run('program:' + target);

		done();
	});

	grunt.registerTask('cxx', 'Compile c++ source code', function(name, srcFile) {
		if (!grunt.file.exists(srcFile)) {
			grunt.log.warn('Source file "' + srcFile + '" not found.');
			return false;
		}
		var options = grunt.config.get('cxx.' + name);

		var outFile = path.join(options.outDir, path.basename(srcFile, ".cpp") + ".obj");

		// ${CXX} ${CXXFLAGS} ${CPPFLAGS} ${INCLUDES} ${DEFINES} ${SRC}
		var cxx = util.format("%s %s %s -I %s -c  %s -o %s",
							  "c:\\Tools\\LLVM\\bin\\clang-cl.exe",
							  options.cxxflags.join(" "),
							  options.cppflags.join(" "),
							  options.includes.join(" -I "),
							  srcFile,
							  outFile);

		var child = spawn('cmd.exe', ['/S', '/C', cxx]);

		if (child.stdout && child.stdout.length > 0) {
			grunt.log.writeln(child.stdout);
		}
		if (child.stderr && child.stderr.length > 0) {
			grunt.log.writeln(child.stderr);
		}
		if (child.status != 0) {
			return false;
		}
	});

	grunt.registerTask('shlib', 'Link shared library', function(name) {
		var options = grunt.config.get('shlib.' + name);

		var libs = ["kernel32.lib", 
		            "user32.lib", 
		            "advapi32.lib", 
		            "shell32.lib"].concat(options.libs);

		var objFiles = grunt.file.expand(path.join(options.outDir, '*.obj')),
		    outFile  = path.join(grunt.buildDir, util.format('lib%s.dll', name)),
		    impFile  = util.format('-implib:build/lib%s.lib', name);

		// ${LINK_CXX} ${LINKFLAGS} ${LIB} ${LDFLAGS}
		var linkCXX = util.format("%s -o  %s %s -link %s %s %s",
								  "c:\\Tools\\LLVM\\bin\\clang-cl.exe",
								  outFile,
								  objFiles.join(" "), 
								  options.ldflags.join(" "),
								  impFile,
								  libs.join(" "));
	
		var child = spawn('cmd.exe', ['/S', '/C', linkCXX]);

		if (child.stdout && child.stdout.length > 0) {
			grunt.log.writeln(child.stdout);
		}
		if (child.stderr && child.stderr.length > 0) {
			grunt.log.writeln(child.stderr);
		}
		if (child.status != 0) {
			return false;
		}
	});

	grunt.registerTask('stlib', 'Create static library', function(name) {
		var options = grunt.config.get('stlib.' + name);

		var objFiles = grunt.file.expand(path.join(options.outDir, '*.obj')),
		    outFile  = path.join(grunt.buildDir, util.format('%s.lib', name));

		var ar = util.format("%s -out:%s %s",
			                 "c:\\Tools\\LLVM\\bin\\llvm-lib.exe",
			                 outFile,
			                 objFiles.join(" "));

		var child = spawn('cmd.exe', ['/S', '/C', ar]);

		if (child.stdout && child.stdout.length > 0) {
			grunt.log.writeln(child.stdout);
		}
		if (child.stderr && child.stderr.length > 0) {
			grunt.log.writeln(child.stderr);
		}
		if (child.status != 0) {
			return false;
		}
	});

	grunt.registerTask('program', 'Link a program file', function(name) {
		var options = grunt.config.get('program.' + name);

		var libs = ["kernel32.lib", 
		            "user32.lib", 
		            "advapi32.lib", 
		            "shell32.lib"].concat(options.libs);

		var objFiles = grunt.file.expand(path.join(options.outDir, '*.obj')),
		    outFile  = path.join(grunt.buildDir, name + '.exe');

		// ${LINK_CXX} ${LINKFLAGS} ${LIB} ${LDFLAGS}
		var linkCXX = util.format("%s -v -o  %s %s -link %s %s",
								  "c:\\Tools\\LLVM\\bin\\clang-cl.exe",
								  outFile,
								  objFiles.join(" "), 
								  options.ldflags.join(" "),
								  libs.join(" "));

		var child = spawn('cmd.exe', ['/S', '/C', linkCXX]);

		if (child.stdout && child.stdout.length > 0) {
			grunt.log.writeln(child.stdout);
		}
		if (child.stderr && child.stderr.length > 0) {
			grunt.log.writeln(child.stderr);
		}
		if (child.status != 0) {
			return false;
		}
	});

};
