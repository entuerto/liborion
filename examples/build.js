/*
   examples
*/
module.exports = function(grunt) {
   "use strict";

   grunt.config.merge({
      cxxshlib: {
         options: {
            cxxflags : ["-Wall"],
            includes : [".", "include/", "lib/", "/Tools/boost_1_61_0"],
            defines  : ["ORION_SHARED_EXPORTS"],
            libPaths : ["build/"]
         },
         'module-example': {
            options: {
               includes: ['.', 'examples/', 'include/'],
               libs    : ['orion']
            },
            src: ['examples/module-example-lib.cpp']
         }
      },
      cxxprogram: {
         options: {
            libs    : ["orion"],
            libPaths: ["build/"]
         },
         'log-example': {
            options: {
               cxxflags : ["-Wall"],
               includes : ['.', 'examples/', 'include/', "/Tools/boost_1_61_0"]
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
         },
         'hello-server': {
            options: {
               cxxflags : ["-Wall"],
               includes : ['.', 'examples/', 'include/', "/Tools/boost_1_61_0"],
               libs     : ["libjson", "orion", "orion-net"]
            },
            src : ['examples/hello-server.cpp']
         },
         'add-json-rpc-server': {
            options: {
               cxxflags : ["-Wall"],
               includes : ['.', 'examples/', 'include/', "deps/", "/Tools/boost_1_61_0"],
               libs     : ["libjson", "orion", "orion-net", "orion-ws"]
            },
            src : ['examples/add-json-rpc-server.cpp']
         }
      }
   });

};
