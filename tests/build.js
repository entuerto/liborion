/*
Unit-tests
*/
module.exports = function(grunt) {
   "use strict";

   grunt.config.merge({
      cxxprogram: {
         options: {
            libs    : ["orion"],
            libPaths: ["build/"]
         },
         'test-encoding': {
            options: {
               cxxflags: ["-Wall"],
               includes: [".", "include/"]
            },
            src : ['tests/test-encoding.cpp']
         },
         'test-jsonrpc': {
            options: {
               cxxflags : ["-Wall"],
               includes : [".", "include/", "lib/", "deps/"],
               libs     : ["libjson", "orion", "orion-net", "orion-ws"]
            },
            src : ['tests/test-jsonrpc.cpp']
         },
         'test-logger': {
            options: {
               cxxflags : ["-Wall"],
               includes : [".", "include/", "lib/", "/Tools/boost_1_61_0"]
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
         'test-units': {
            options: {
               cxxflags : ["-Wall"],
               includes : [".", "include/"]
            },
            src :  ['tests/test-units.cpp'],
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
         }
      }
   });

};
