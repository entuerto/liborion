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
				src : ['deps/mongoose/mongoose.c']
			}
		},
		cxxstlib: {
			json: {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "deps/jsoncpp"],
					defines  : ["ORION_SHARED_EXPORTS"]
				},
				src :  ['deps/jsoncpp/jsoncpp.cpp']
			}
		},
		cxxshlib: {
			options: {
				cxxflags : ["-Wall"],
				includes : [".", "include/", "lib/"],
				defines  : ["ORION_SHARED_EXPORTS"],
				libPaths : ["build/"]
				},
			orion: {
				options: {
					libs : ["psapi", "ntdll", "rpcrt4"]
				},
				src :  ['lib/ArgumentExceptions.cpp',
						'lib/DateUtils.cpp',
						'lib/Encoding.cpp',
						'lib/Exception.cpp',
						'lib/Module.cpp',
						'lib/Module-<%= process.platform %>.cpp',
						'lib/ModuleException.cpp',
						'lib/NotImplementedException.cpp',
						'lib/StringUtils.cpp',
						'lib/SystemInfo.cpp',
						'lib/SystemInfo-<%= process.platform %>.cpp',
						'lib/Timer.cpp',
						'lib/Uuid-<%= process.platform %>.cpp',
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
			'orion-plugin': {
				options: {
					libs : ['orion']
				},
				src :  ['lib/plugin/Extension.cpp',
                        'lib/plugin/PlugIn.cpp',
                        'lib/plugin/PlugInException.cpp',
                        'lib/plugin/PlugInManager.cpp']
			},
			'orion-net': {
				options: {
					libs : ["orion", "ws2_32"]
				},
				src :  ['lib/net/IP.cpp',
						'lib/net/IPAddress.cpp',
						'lib/net/TcpConnection.cpp',
						'lib/net/TcpListener.cpp']	
			},
			'orion-ws': {
				options: {
					includes : ["deps/", "include/", "lib/"],
					libs : ["json", 
					        "mongoose", 
					        "orion",
					        "psapi", 
					        "ws2_32", 
					        "rpcrt4"]
				},
				src :  ['lib/ws/impl/MongooseRequest.cpp',
						'lib/ws/impl/MongooseHttpServer.cpp',
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
			},
			'module-example': {
				options: {
					includes : ['.', 'examples/', 'include/'],
					libs     : ['orion']
				},
				src :  ['examples/module-example-lib.cpp']
			}	
		},
		cxxprogram: {
			options: {
				libs     : ["orion"],
				libPaths : ["build/"]
			},
			'test-encoding': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/"]
				},
				src : ['tests/test-encoding.cpp']
			},
			'test-jsonrpc': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/", "lib/", "deps/"],
					libs     : ["json", "orion", "orion-ws"]
				},
				src : ['tests/test-jsonrpc.cpp']
			},
			'test-logger': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/", "lib/"]
				},
				src : ['tests/test-logger.cpp']
			},
			'test-net': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/"],
					libs     : ["orion", "orion-net"]
				},
				src : ['tests/test-net.cpp']
			},
			'test-string': {
				options: {
					cxxflags : ["-Wall"],
					includes : [".", "include/", "lib/"]
				},
				src : ['tests/test-string.cpp']
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
			'log-example': {
				options: {
					cxxflags : ["-Wall"],
					includes : ['.', 'examples/', 'include/']
				},
				src : ['examples/log-example.cpp']
			},
			'module-example': {
				options: {
					cxxflags : ["-Wall"],
					includes : ['.', 'examples/', 'include/']
				},
				src : ['examples/module-example.cpp']
			},
			'net-example': {
				options: {
					cxxflags : ["-Wall"],
					includes : ['.', 'examples/', 'include/'],
					libs     : ["Iphlpapi"]
				},
				src : ['examples/net-example.cpp']
			},
			'signal-example': {
				options: {
					cxxflags : ["-Wall"],
					includes : ['.', 'examples/', 'include/']
				},
				src : ['examples/signal-example.cpp']
			},
			'system-info': {
				options: {
					cxxflags : ["-Wall"],
					includes : ['.', 'examples/', 'include/'],
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
