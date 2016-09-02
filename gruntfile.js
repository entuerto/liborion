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
            src: ['deps/mongoose/mongoose.c']
         }
      },
      cxxstlib: {
         json: {
            options: {
               cxxflags : ["-Wall"],
               includes : [".", "deps/jsoncpp"],
               defines  : ["ORION_SHARED_EXPORTS"]
            },
            src: ['deps/jsoncpp/jsoncpp.cpp']
         }
      },
      cxxshlib: {
         options: {
            cxxflags : ["-Wall"],
            includes : [".", "include/", "lib/", "/Tools/boost_1_61_0"],
            defines  : ["ORION_SHARED_EXPORTS"],
            libPaths : ["build/"]
            },
         orion: {
            options: {
               libs: ["psapi", "ntdll", "rpcrt4"]
            },
            src: ['lib/ArgumentExceptions.cpp',
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
               libs: ['orion']
            },
            src: ['lib/plugin/Extension.cpp',
                  'lib/plugin/PlugIn.cpp',
                  'lib/plugin/PlugInException.cpp',
                  'lib/plugin/PlugInManager.cpp']
         },
         'orion-net': {
            options: {
               includes: ["deps/", "include/", "lib/", "/Tools/boost_1_61_0"],
               libs: ["libmongoose",
                      "orion", 
                      "ws2_32"]
            },
            src: ['lib/net/impl/MongooseRequest.cpp',
                  'lib/net/impl/MongooseHttpServer.cpp',
                  'lib/net/IP.cpp',
                  'lib/net/IPAddress.cpp',
                  'lib/net/Request.cpp',
                  'lib/net/RequestListener.cpp',
                  'lib/net/Response.cpp',
                  'lib/net/HttpServer.cpp',
                  'lib/net/TcpConnection.cpp',
                  'lib/net/TcpListener.cpp'] 
         },
         'orion-ws': {
            options: {
               includes: ["deps/", "include/", "lib/", "/Tools/boost_1_61_0"],
               libs: ["libjson", 
                      "orion",
                      "orion-net",
                      "psapi", 
                      "rpcrt4"]
            },
            src: ['lib/ws/RpcError.cpp',
                  'lib/ws/JsonRpcError.cpp',
                  'lib/ws/JsonRpcMethodWrapper.cpp',
                  'lib/ws/RpcRequestListener.cpp',
                  'lib/ws/JsonRpcRequestListener.cpp']   
         }  
      },
      clean: {
         build: ['build/**', '!build/config.json'],
         docs:  ['docs/*/']
      }
   });

   grunt.loadTasks('tests');
   grunt.loadTasks('examples');

   // Load this plugin's task(s).
   grunt.loadTasks('tasks');
   grunt.loadNpmTasks('grunt-contrib-clean');

   grunt.registerTask('default', ['cstlib', 
                                  'cxxstlib', 
                                  'cxxshlib', 
                                  'cxxprogram']);
};
