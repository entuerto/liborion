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
		let config = grunt.cxxshlibConfig(this.options({
			ldflags : ["-dll", "-dynamicbase", '-libpath:build/'],
			outDir  : path.join(grunt.buildDir, this.target)
		}));

		grunt.file.write('config.json', JSON.stringify(config, null, '\t'));

		if (!fs.existsSync(config.options.outDir))
			fs.mkdirSync(config.options.outDir);

		grunt.config.set('cxx.' + this.target, config);
		grunt.config.set('shlib.' + this.target, config);

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
		let config = grunt.cxxstlibConfig(this.options({
			outDir  : path.join(grunt.buildDir, this.target)
		}));

		if (!fs.existsSync(config.options.outDir))
			fs.mkdirSync(config.options.outDir);

		grunt.config.set('cxx.' + this.target, config);
		grunt.config.set('stlib.' + this.target, config);

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
		let config = grunt.cxxprogramConfig(this.options({
			ldflags : ['-libpath:build/'],
			outDir  : path.join(grunt.buildDir, this.target)
		}));

		grunt.verbose.writeflags(config, 'Config');

		if (!fs.existsSync(config.options.outDir))
			fs.mkdirSync(config.options.outDir);

		grunt.config.set('cxx.' + this.target, config);
		grunt.config.set('program.' + this.target, config);

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
		let config  = grunt.config.get('cxx.' + name);
		var options = config.options;

		var outFile = path.join(options.outDir, path.basename(srcFile, ".cpp") + ".obj");

		if (grunt.file.exists(outFile) &&
		    fs.lstatSync(srcFile).mtime.getTime() <= 
		    fs.lstatSync(outFile).mtime.getTime())
		   return true; 

		// '${CXX} ${CXXFLAGS} ${CPPFLAGS} ${DEFINES} ${INCLUDES} -c ${SRC} -o ${OUT}'
		var cxx = config.templates.cxx({
			CXX      : config.programs.CXX,
			CXXFLAGS : options.cxxflags.join(" "),
			CPPFLAGS : options.cppflags.join(" "),
			DEFINES  : joinWith(options.defines, config.patterns.define),
			INCLUDES : joinWith(options.includes, config.patterns.include), 
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
		let config  = grunt.config.get('shlib.' + name);
		var options = config.options;

		var libs = ["kernel32.lib", 
		            "user32.lib", 
		            "advapi32.lib", 
		            "shell32.lib"].concat(options.libs);

		var objFiles = grunt.file.expand(path.join(options.outDir, '*.obj')),
		    outFile  = path.join(grunt.buildDir, util.format(config.patterns.shlib, name)),
		    impFile  = path.join(grunt.buildDir, util.format(config.patterns.implib, name));

		// ${LINK} -o ${OUT} ${OBJS} -link ${IMPLIB} ${LDFLAGS} ${LDLIBS} 
		var link = config.templates.link({
			LINK    : config.programs.LINK,
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
		let config  = grunt.config.get('stlib.' + name);
		var options = config.options;

		var objFiles = grunt.file.expand(path.join(options.outDir, '*.obj')),
		    outFile  = path.join(grunt.buildDir, util.format(config.patterns.stlib, name));

		// ${AR} -out:${OUT} ${OBJS} 
		var ar = config.templates.ar({
			AR   : config.programs.AR,
			OUT  : outFile,
			OBJS : objFiles.join(" ")
		});
		grunt.verbose.writeln(ar);

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
		let config  = grunt.config.get('program.' + name);
		var options = config.options;

		var libs = ["kernel32.lib", 
		            "user32.lib", 
		            "advapi32.lib", 
		            "shell32.lib"].concat(options.libs);

		var objFiles = grunt.file.expand(path.join(options.outDir, '*.obj')),
		    outFile  = path.join(grunt.buildDir, name + '.exe');

		// ${LINK} -o ${OUT} ${OBJS} -link ${LDFLAGS} ${LDLIBS} 
		var link = config.templates.link({
			LINK    : config.programs.LINK,
			LDLIBS  : libs.join(" "),
			LDFLAGS : "-libpath:build/", //options.ldflags.join(" "),
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
