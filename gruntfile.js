/*
liborion compilation
*/
module.exports = function(grunt) {
	"use strict";

	require('time-grunt')(grunt);

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
			options: {
				cxxflags : ["-Wall"],
				includes : [".", "include/", "lib/"],
				defines  : ["ORION_SHARED_EXPORTS"],
				libPaths : ["build"]
				},
			orion: {
				options: {
					libs : ["psapi.lib", "ntdll.lib", "rpcrt4.lib"]
				},
				src :  ['lib/ArgumentExceptions.cpp',
						'lib/DateUtils.cpp',
						'lib/Encoding.cpp',
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
			'orion-net': {
				options: {
					libs : ["ws2_32.lib", "liborion.dll.lib"]
				},
				src :  ['lib/net/IP.cpp',
						'lib/net/IPAddress.cpp',
						'lib/net/TcpConnection.cpp',
						'lib/net/TcpListener.cpp']	
			},
			'orion-ws': {
				options: {
					libs : ["psapi.lib", "ws2_32.lib", "rpcrt4.lib", "libjson.lib", "libmongoose.lib", "liborion.dll.lib"]
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
			options: {
				libs     : ["liborion.dll.lib"],
				libPaths : ["build"]
			},
			'test-logger': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/", "lib/"]
				},
				src : ['tests/test-logger.cpp']
			},
			'test-unittest': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/", "lib/"]
				},
				src :  ['tests/test-unittest.cpp',
						'tests/unittest/ClassTest.cpp',
						'tests/unittest/ClassTestResult.cpp',
						'tests/unittest/EvalMacros.cpp'],
			},
			'test-string': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/", "lib/"]
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
			'test-net': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/"],
					libs     : ["liborion.dll.lib", "liborion-net.dll.lib"]
				},
				src : ['tests/test-net.cpp']
			},
			'test-encoding': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/"]
				},
				src : ['tests/test-encoding.cpp']
			},
			'signal-example': {
				options: {
					cxxflags : ["-Wall"],
					includes : ['.', 'examples/', 'include/']
				},
				src : ['examples/signal-example.cpp']
			},
			'log-example': {
				options: {
					cxxflags : ["-Wall"],
					includes : ['.', 'examples/', 'include/']
				},
				src : ['examples/log-example.cpp']
			},
			'system-info': {
				options: {
					cxxflags : ["-Wall"],
					includes : ['.', 'examples/', 'include/'],
				},
				src : ['examples/system-info.cpp']
			},
			'net-example': {
				options: {
					cxxflags : ["-Wall"],
					includes : ['.', 'examples/', 'include/'],
					libs     : ["Iphlpapi.lib"]
				},
				src : ['examples/net-example.cpp']
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
