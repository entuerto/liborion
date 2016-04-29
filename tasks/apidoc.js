/*
 * Copyright (c) 2016 Tomas Palazuelos
 * Licensed under the MIT license.
 */
module.exports = function(grunt) {
	"use strict";

	var spawn = require('child_process').spawnSync;
	var path = require('path');

	grunt.registerTask('apidoc', 'Generate API reference documentation', function() {

		var doxyfile = path.join(grunt.docsDir, 'Doxyfile'),
		    doxygen = 'c:\\Tools\\doxygen\\bin\\doxygen.exe';

		var child = spawn('cmd.exe', ['/S', '/C', doxygen, doxyfile]);

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
