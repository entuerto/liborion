/*
liborion compilation
*/
module.exports = function(grunt) {
	"use strict";

	grunt.initConfig({
		pkg: grunt.file.readJSON('package.json'),
		cstlib: {
			mongoose: {
				options: {
					cxxflags : ["-Wall"],
					includes : ["."],
					defines  : ["ORION_SHARED_EXPORTS"]
				},
				src : ['lib/ws/mongoose/mongoose.c']
			}
		},
		cxxstlib: {
			json: {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "lib/ws"],
					defines  : ["ORION_SHARED_EXPORTS"]
				},
				src :  ['lib/ws/json/json_reader.cpp',
						'lib/ws/json/json_value.cpp',
						'lib/ws/json/json_writer.cpp']
			}
		},
		cxxshlib: {
			orion: {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/", "lib/"],
					defines  : ["ORION_SHARED_EXPORTS"],
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
					cxxflags : ["-Wall"],
					includes : [".", "include/", "lib/"],
					defines  : ["ORION_SHARED_EXPORTS"],
					libs     : ["psapi.lib", "ws2_32.lib", "rpcrt4.lib", "libjson.lib", "libmongoose.lib", "liborion.dll.lib"]
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
					cxxflags : ["-Wall"],
					includes : [".", "include/", "lib/"],
					libs     : ["liborion.dll.lib"]
				},
				src : ['tests/test-logger.cpp']
			},
			'test-unittest': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/", "lib/"],
					libs     : ["liborion.dll.lib"]
				},
				src :  ['tests/test-unittest.cpp',
						'tests/unittest/ClassTest.cpp',
						'tests/unittest/ClassTestResult.cpp',
						'tests/unittest/EvalMacros.cpp'],
			},
			'test-string': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/", "lib/"],
					libs     : ["liborion.dll.lib"]
				},
				src : ['tests/test-string.cpp']
			},
			'test-jsonrpc': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/", "lib/"],
					libs     : ["libjson.lib", "liborion.dll.lib", "liborion-ws.dll.lib"]
				},
				src : ['tests/test-jsonrpc.cpp']
			},
			'signal-example': {
				options: {
					cxxflags : ["-Wall"],
					includes : ['.', 'examples/', 'include/'],
					libs     : ["liborion.dll.lib"]
				},
				src : ['examples/signal-example.cpp']
			},
			'log-example': {
				options: {
					cxxflags : ["-Wall"],
					includes : ['.', 'examples/', 'include/'],
					libs     : ["liborion.dll.lib"]
				},
				src : ['examples/log-example.cpp']
			},
			'system-info': {
				options: {
					cxxflags : ["-Wall"],
					includes : ['.', 'examples/', 'include/'],
					libs     : ["liborion.dll.lib"]
				},
				src : ['examples/system-info.cpp']
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

	grunt.registerTask('default', ['cstlib', 
		                           'cxxstlib', 
		                           'cxxshlib', 
		                           'cxxprogram']);

};
