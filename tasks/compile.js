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

	grunt.registerMultiTask('cshlib', 'Compile and link a C shared library', function() {
		let options = this.options({
			outDir : path.join(grunt.buildDir, this.target)
		});

		if (!fs.existsSync(options.outDir))
			fs.mkdirSync(options.outDir);

		grunt.config.set('cc.' + this.target, options);
		grunt.config.set('shlib.' + this.target, options);

		var done = this.async();

		var target = this.target;
		// Iterate over all src files.
		var taskList = this.filesSrc.map(function(srcFile) {
			return 'cc:' + target + ':' + srcFile;
		});

		grunt.task.run(taskList);
		grunt.task.run('shlib:' + target);

		done();
	});

	grunt.registerMultiTask('cxxshlib', 'Compile and link a C++ shared library', function() {
		let options = this.options({
			outDir : path.join(grunt.buildDir, this.target)
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

	grunt.registerMultiTask('cstlib', 'Compile and create static library', function() {
		let options = this.options({
			outDir : path.join(grunt.buildDir, this.target)
		});

		if (!fs.existsSync(options.outDir))
			fs.mkdirSync(options.outDir);

		grunt.config.set('cc.' + this.target, options);
		grunt.config.set('stlib.' + this.target, options);

		var done = this.async();

		var target = this.target;
		// Iterate over all src files.
		var taskList = this.filesSrc.map(function(srcFile) {
			return 'cc:' + target + ':' + srcFile;
		});

		grunt.task.run(taskList);
		grunt.task.run('stlib:' + target);

		done();
	});

	grunt.registerMultiTask('cxxstlib', 'Compile and create static library', function() {
		let options = this.options({
			outDir : path.join(grunt.buildDir, this.target)
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

	grunt.registerMultiTask('cprogram', 'Compile and link a program', function() {
		let options = this.options({
			outDir : path.join(grunt.buildDir, this.target)
		});

		if (!fs.existsSync(options.outDir))
			fs.mkdirSync(options.outDir);

		grunt.config.set('cc.' + this.target, options);
		grunt.config.set('program.' + this.target, options);

		var done = this.async();

		var target = this.target;
		// Iterate over all src files.
		var taskList = this.filesSrc.map(function(srcFile) {
			return 'cc:' + target + ':' + srcFile;
		});

		grunt.task.run(taskList);
		grunt.task.run('program:' + target);
                     
		done();
	});

	grunt.registerMultiTask('cxxprogram', 'Compile and link a program', function() {
		let options = this.options({
			outDir : path.join(grunt.buildDir, this.target)
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

	grunt.registerTask('cc', 'Compile c source code', function(name, srcFile) {
		if (!grunt.file.exists(srcFile)) {
			grunt.log.warn('Source file "' + srcFile + '" not found.');
			return false;
		}

		let config   = grunt.build.config('cc', grunt.config.get('cc.' + name)),
		    options  = config.options,
		    patterns = grunt.build.patterns('cc');

		let outFile = path.join(options.outDir, 
			                    util.format('%s.%s', path.parse(srcFile).name, 
			                    	                 patterns.objExt));

		if (grunt.file.exists(outFile) &&
		    fs.lstatSync(srcFile).mtime.getTime() <= 
		    fs.lstatSync(outFile).mtime.getTime())
		   return true; 

		// '${CC} ${CFLAGS} ${CPPFLAGS} ${DEFINES} ${INCLUDES} -c ${SRC} -o ${OUT}'
		let cc = config.cmd({
			CC       : config.CC,
			CFLAGS   : options.cxxflags.join(" "),
			CPPFLAGS : options.cppflags.join(" "),
			DEFINES  : joinWith(options.defines, patterns.define),
			INCLUDES : joinWith(options.includes, patterns.include), 
			SRC      : srcFile,
			OUT      : outFile
		});
		grunt.verbose.writeln(cc);

		var child = spawn('cmd.exe', ['/S', '/C', cc]);

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

	grunt.registerTask('cxx', 'Compile c++ source code', function(name, srcFile) {
		if (!grunt.file.exists(srcFile)) {
			grunt.log.warn('Source file "' + srcFile + '" not found.');
			return false;
		}

		let config   = grunt.build.config('cxx', grunt.config.get('cxx.' + name)),
		    options  = config.options,
		    patterns = grunt.build.patterns('cxx');

		let outFile = path.join(options.outDir, 
			                    util.format('%s.%s', path.parse(srcFile).name, 
			                    	                 patterns.objExt));

		if (grunt.file.exists(outFile) &&
		    fs.lstatSync(srcFile).mtime.getTime() <= 
		    fs.lstatSync(outFile).mtime.getTime())
		   return true; 

		// '${CXX} ${CXXFLAGS} ${CPPFLAGS} ${DEFINES} ${INCLUDES} -c ${SRC} -o ${OUT}'
		var cxx = config.cmd({
			CXX      : config.CXX,
			CXXFLAGS : options.cxxflags.join(" "),
			CPPFLAGS : options.cppflags.join(" "),
			DEFINES  : joinWith(options.defines, patterns.define),
			INCLUDES : joinWith(options.includes, patterns.include), 
			SRC      : srcFile,
			OUT      : outFile
		});
		grunt.verbose.writeln(cxx);

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
		let config   = grunt.build.config('shlib', grunt.config.get('shlib.' + name)),
		    options  = config.options,
		    patterns = grunt.build.patterns('shlib');

		var libs = ["kernel32.lib", 
		            "user32.lib", 
		            "advapi32.lib", 
		            "shell32.lib"].concat(options.libs);

		var objFiles = grunt.file.expand(path.join(options.outDir, patterns.objGlob)),
		    outFile  = path.join(grunt.buildDir, util.format(patterns.shlib, name)),
		    impFile  = path.join(grunt.buildDir, util.format(patterns.implib, name));

		// ${LINK} -o ${OUT} ${OBJS} -link ${IMPLIB} ${LDFLAGS} ${LDLIBS} 
		var link = config.cmd({
			LINK    : config.LINK,
			LDLIBS  : libs.join(" "),
			LDFLAGS : options.ldflags.join(" "),
			IMPLIB  : impFile,
			OBJS    : objFiles.join(" "),
			OUT     : outFile
		});
		grunt.verbose.writeln(link);

		var child = spawn('cmd.exe', ['/S', '/C', link]);

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
		let config   = grunt.build.config('stlib', grunt.config.get('stlib.' + name)),
		    options  = config.options,
		    patterns = grunt.build.patterns('stlib');

		let objFiles = grunt.file.expand(path.join(options.outDir, patterns.objGlob)),
		    outFile  = path.join(grunt.buildDir, util.format(patterns.stlib, name));

		// ${AR} -out:${OUT} ${OBJS} 
		let ar = config.cmd({
			AR   : config.AR,
			OUT  : outFile,
			OBJS : objFiles.join(" ")
		});
		grunt.verbose.writeln(ar);

		let child = spawn('cmd.exe', ['/S', '/C', ar]);

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
		let config   = grunt.build.config('program', grunt.config.get('program.' + name)),
		    options  = config.options,
		    patterns = grunt.build.patterns('program');

		var libs = ["kernel32.lib", 
		            "user32.lib", 
		            "advapi32.lib", 
		            "shell32.lib"].concat(options.libs);

		var objFiles = grunt.file.expand(path.join(options.outDir, patterns.objGlob)),
		    outFile  = path.join(grunt.buildDir, util.format(patterns.prog, name));

		// ${LINK} -o ${OUT} ${OBJS} -link ${LDFLAGS} ${LDLIBS} 
		var link = config.cmd({
			LINK    : config.LINK,
			LDLIBS  : libs.join(" "),
			LDFLAGS : options.ldflags.join(" "),
			OBJS    : objFiles.join(" "),
			OUT     : outFile
		});
		grunt.verbose.writeln(link);

		var child = spawn('cmd.exe', ['/S', '/C', link]);

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

	function joinWith(array, template) {
		return array.map(function(currentValue) { 
			return util.format(template, currentValue)
		}).join(" ");
	}

};
