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

	function compileCC(srcFile, trgtOpts) {
		let config   = grunt.build.config('cc', trgtOpts),
		    options  = config.options,
		    patterns = grunt.build.patterns();

		let outFile = path.join(options.outDir, 
			                    util.format('%s.%s', path.parse(srcFile).name, 
			                    	                 patterns.objExt));

		if (grunt.file.exists(outFile) &&
		    fs.lstatSync(srcFile).mtime.getTime() <= 
		    fs.lstatSync(outFile).mtime.getTime())
		   return true; 

		grunt.log.writeln(util.format('cc: %s\t->\t%s', srcFile, outFile));

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
	}

	function compileCXX(srcFile, trgtOpts) {
		let config   = grunt.build.config('cxx', trgtOpts),
		    options  = config.options,
		    patterns = grunt.build.patterns();

		let outFile = path.join(trgtOpts.outDir, 
			                    util.format('%s.%s', path.parse(srcFile).name, 
			                    	                 patterns.objExt));

		if (grunt.file.exists(outFile) &&
		    fs.lstatSync(srcFile).mtime.getTime() <= 
		    fs.lstatSync(outFile).mtime.getTime())
		   return true; 

		grunt.log.writeln(util.format('cxx: %s\t->\t%s', srcFile, outFile));

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

		return true;
	}

	function linkSharedLib(target, trgtOpts) {
		let config   = grunt.build.config('shlib', trgtOpts),
		    options  = config.options,
		    patterns = grunt.build.patterns();

		var libs = options.stdLibs.concat(options.libs);

		var objFiles = grunt.file.expand(path.join(options.outDir, patterns.objGlob)),
		    outFile  = path.join(grunt.buildDir, util.format(patterns.shlib, target))

		options.ldflags.push(joinWith(options.libPaths, patterns.libpath));

		grunt.log.writeln(util.format('shlib: Linking %s', outFile));

		// ${LINK} -o ${OUT} ${OBJS} -link ${IMPLIB} ${LDFLAGS} ${LDLIBS} 
		var link = config.cmd({
			LINK    : config.LINK,
			LDLIBS  : libs.join(" "),
			LDFLAGS : options.ldflags.join(" "),
			IMPLIB  : util.format(patterns.implib, grunt.buildDir, target),
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

		return true;
	}

	function linkStaticLib(target, trgtOpts) {
		let config   = grunt.build.config('stlib', trgtOpts),
		    options  = config.options,
		    patterns = grunt.build.patterns();

		let objFiles = grunt.file.expand(path.join(options.outDir, patterns.objGlob)),
		    outFile  = path.join(grunt.buildDir, util.format(patterns.stlib, target));

		grunt.log.writeln(util.format('stlib: Creating archive %s', outFile));

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

		return true;
	}

	function linkProgram(target, trgtOpts) {
		let config   = grunt.build.config('program', trgtOpts),
		    options  = config.options,
		    patterns = grunt.build.patterns();

		var libs = options.stdLibs.concat(options.libs);

		var objFiles = grunt.file.expand(path.join(options.outDir, patterns.objGlob)),
		    outFile  = path.join(grunt.buildDir, util.format(patterns.prog, target));

		options.ldflags.push(joinWith(options.libPaths, patterns.libpath));
		
		grunt.log.writeln(util.format('program: Creating %s', outFile));

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

		return true;
	}

	grunt.registerMultiTask('cshlib', 'Compile and link a C shared library', function() {
		let options = this.options({
			outDir : path.join(grunt.buildDir, this.target)
		});

		if (!fs.existsSync(options.outDir))
			fs.mkdirSync(options.outDir);

		var done = this.async();

		if (!this.files.length) {
			return done();
		}

		// Iterate over all src files.
		this.filesSrc.forEach(function(srcFile) {
			if (!grunt.file.exists(srcFile)) {
				grunt.log.warn('Source file "' + srcFile + '" not found.');
				return false;
			}

			if (!compileCC(srcFile, options)) {
				return done(false);
			}
		});

		if (!linkSharedLib(this.target, options)) {
			return done(false);
		}
	
		done();
	});

	grunt.registerMultiTask('cxxshlib', 'Compile and link a C++ shared library', function() {
		let options = this.options({
			outDir : path.join(grunt.buildDir, this.target)
		});

		if (!fs.existsSync(options.outDir))
			fs.mkdirSync(options.outDir);

		var done = this.async();

		if (!this.files.length) {
			return done();
		}

		// Iterate over all src files.
		this.filesSrc.forEach(function(srcFile) {
			if (!grunt.file.exists(srcFile)) {
				grunt.log.warn('Source file "' + srcFile + '" not found.');
				return false;
			}

			if (!compileCXX(srcFile, options)) {
				return done(false);
			}
		});

		if (!linkSharedLib(this.target, options)) {
			return done(false);
		}
	
		done();
	});

	grunt.registerMultiTask('cstlib', 'Compile and create static library', function() {
		let options = this.options({
			outDir : path.join(grunt.buildDir, this.target)
		});

		if (!fs.existsSync(options.outDir))
			fs.mkdirSync(options.outDir);

		var done = this.async();

		if (!this.files.length) {
			return done();
		}

		// Iterate over all src files.
		this.filesSrc.forEach(function(srcFile) {
			if (!grunt.file.exists(srcFile)) {
				grunt.log.warn('Source file "' + srcFile + '" not found.');
				return false;
			}

			if (!compileCC(srcFile, options)) {
				return done(false);
			}
		});

		if (!linkStaticLib(this.target, options)) {
			return done(false);
		}
	
		done();
	});

	grunt.registerMultiTask('cxxstlib', 'Compile and create static library', function() {
		let options = this.options({
			outDir : path.join(grunt.buildDir, this.target)
		});

		if (!fs.existsSync(options.outDir))
			fs.mkdirSync(options.outDir);

		var done = this.async();

		if (!this.files.length) {
			return done();
		}

		// Iterate over all src files.
		this.filesSrc.forEach(function(srcFile) {
			if (!grunt.file.exists(srcFile)) {
				grunt.log.warn('Source file "' + srcFile + '" not found.');
				return false;
			}

			if (!compileCXX(srcFile, options)) {
				return done(false);
			}
		});

		if (!linkStaticLib(this.target, options)) {
			return done(false);
		}
	
		done();
	});

	grunt.registerMultiTask('cprogram', 'Compile and link a program', function() {
		let options = this.options({
			outDir : path.join(grunt.buildDir, this.target)
		});

		if (!fs.existsSync(options.outDir))
			fs.mkdirSync(options.outDir);

		var done = this.async();

		if (!this.files.length) {
			return done();
		}

		// Iterate over all src files.
		this.filesSrc.forEach(function(srcFile) {
			if (!grunt.file.exists(srcFile)) {
				grunt.log.warn('Source file "' + srcFile + '" not found.');
				return false;
			}

			if (!compileCC(srcFile, options)) {
				return done(false);
			}
		});

		if (!linkProgram(this.target, options)) {
			return done(false);
		}
	
		done();
	});

	grunt.registerMultiTask('cxxprogram', 'Compile and link a program', function() {
		let options = this.options({
			outDir : path.join(grunt.buildDir, this.target)
		});

		if (!fs.existsSync(options.outDir))
			fs.mkdirSync(options.outDir);

		var done = this.async();

		if (!this.files.length) {
			return done();
		}

		// Iterate over all src files.
		this.filesSrc.forEach(function(srcFile) {
			if (!grunt.file.exists(srcFile)) {
				grunt.log.warn('Source file "' + srcFile + '" not found.');
				return false;
			}

			if (!compileCXX(srcFile, options)) {
				return done(false);
			}
		});

		if (!linkProgram(this.target, options)) {
			return done(false);
		}
	
		done();
	});

	function joinWith(array, template) {
		return array.map(function(currentValue) { 
			return util.format(template, currentValue)
		}).join(" ");
	}

};
