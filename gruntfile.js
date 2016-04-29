/*
liborion compilation
*/
module.exports = function(grunt) {
	"use strict";

	grunt.initConfig({
		pkg: grunt.file.readJSON('package.json'),
		cxxstlib: {
			mongoose: {
				options: {
					cxxflags : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
					cppflags : ["-D __MINGW64__", "-D WIN32", "-D ORION_SHARED_EXPORTS"],
					includes : ["."],
				},
				src : ['lib/ws/mongoose/mongoose.c']
			},
			json: {
				options: {
					cxxflags : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
					cppflags : ["-D __MINGW64__", "-D WIN32", "-D ORION_SHARED_EXPORTS"],
					includes : [".", "lib/ws"],
				},
				src :  ['lib/ws/json/json_reader.cpp',
						'lib/ws/json/json_value.cpp',
						'lib/ws/json/json_writer.cpp']
			}
		},
		cxxshlib: {
			orion: {
				options: {
					cxxflags : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
					cppflags : ["-D __MINGW64__", "-D WIN32", "-D ORION_SHARED_EXPORTS"],
					includes : [".", "include/", "lib/"],
					libs     : ["psapi.lib", "ntdll.lib", "rpcrt4.lib"]
				},
				src :  ['lib/ArgumentExceptions.cpp',
						'lib/DateUtils.cpp',
						'lib/Exception.cpp',
						'lib/Module.cpp',
						'lib/Module-win32.cpp',
						'lib/ModuleException.cpp',
						'lib/NotImplementedException.cpp',
						'lib/StringUtils.cpp',
						'lib/SystemInfo.cpp',
						'lib/SystemInfo-win32.cpp',
						'lib/Timer.cpp',
						'lib/Uuid-win32.cpp',
						'lib/logging/LogEndRecord.cpp',
						'lib/logging/LogExceptionRecord.cpp',
						'lib/logging/LogFunction.cpp',
						'lib/logging/LogRecord.cpp',
						'lib/logging/LogStartRecord.cpp',
						'lib/logging/LogSystemInfoRecord.cpp',
						'lib/logging/Logger.cpp',
						'lib/logging/MultilineFormatter.cpp',
						'lib/logging/OnelineFormatter.cpp',
						'lib/logging/OnelineWithSourceInfoFormatter.cpp',
						'lib/logging/OutputHandler.cpp',
						'lib/logging/StreamOutputHandler.cpp',
						'lib/unittest/Test.cpp',
						'lib/unittest/TestResult.cpp',
						'lib/unittest/TestResultItem.cpp',
						'lib/unittest/TestStdOutput.cpp']
			},
			'orion-ws': {
				options: {
					cxxflags : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
					cppflags : ["-D __MINGW64__", "-D WIN32", "-D ORION_SHARED_EXPORTS"],
					includes : [".", "include/", "lib/"],
					libs     : ["psapi.lib", "ws2_32.lib", "rpcrt4.lib", "json.lib", "mongoose.lib", "liborion.lib"]
				},
				src :  ['lib/ws/mongoose/MongooseRequest.cpp',
						'lib/ws/mongoose/MongooseHttpServer.cpp',
						'lib/ws/InetAddress.cpp',
						'lib/ws/RpcError.cpp',
						'lib/ws/RpcMethod.cpp',
						'lib/ws/JsonRpcError.cpp',
						'lib/ws/JsonRpcMethodWrapper.cpp',
						'lib/ws/Request.cpp',
						'lib/ws/RequestListener.cpp',
						'lib/ws/RpcRequestListener.cpp',
						'lib/ws/JsonRpcRequestListener.cpp',
						'lib/ws/Response.cpp',
						'lib/ws/HttpServer.cpp']	
			}	
		},
		cxxprogram: {
			'test-logger': {
				options: {
					cxxflags : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
					cppflags : ["-D __MINGW64__", "-D WIN32"],
					includes : [".", "include/", "lib/"],
					libs     : ["liborion.lib"]
				},
				src : ['tests/test-logger.cpp']
			},
			'test-unittest': {
				options: {
					cxxflags : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
					cppflags : ["-D __MINGW64__", "-D WIN32"],
					includes : [".", "include/", "lib/"],
					libs     : ["liborion.lib"]
				},
				src :  ['tests/test-unittest.cpp',
						'tests/unittest/ClassTest.cpp',
						'tests/unittest/ClassTestResult.cpp',
						'tests/unittest/EvalMacros.cpp'],
			},
			'test-string': {
				options: {
					cxxflags : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
					cppflags : ["-D __MINGW64__", "-D WIN32"],
					includes : [".", "include/", "lib/"],
					libs     : ["liborion.lib"]
				},
				src : ['tests/test-string.cpp']
			},
			'test-jsonrpc': {
				options: {
					cxxflags : ["-Wall", "-EHsc", "-GR", "-fms-compatibility-version=19"],
					cppflags : ["-D __MINGW64__", "-D WIN32"],
					includes : [".", "include/", "lib/"],
					libs     : ["json.lib", "liborion.lib", "liborion-ws.lib"]
				},
				src : ['tests/test-jsonrpc.cpp']
			}
		},
		clean: {
			build: ['build/**'],
			docs:  ['docs/*/']
		}
	});

	// Load this plugin's task(s).
	grunt.loadTasks('tasks');
	grunt.loadNpmTasks('grunt-contrib-clean');

	grunt.registerTask('default', ['cxxstlib', 'cxxshlib', 'cxxprogram']);

};
