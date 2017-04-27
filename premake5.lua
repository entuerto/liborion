--
-- premake5.lua
-- liborion configuration file.
-- Copyright (c) 2016 Tomas Palazuelos
--

require "premake/clangcl"
require "premake/boost"
require "premake/utils"
require "premake/gnumake"

workspace "liborion"
   
   version {
      major = 0,
      minor = 1,
      patch = 0
   }

   -- We set the location of the files Premake will generate
   location "build"

   language "C++"

   filter { "system:windows",  "action:gmake or gnumake" }
      toolset "clangcl"

   SetupDefaultConfigurations()

   includedirs { 
      "include", 
      "lib", 
      "deps", 
   }

   FindBoost(1.61)


group "Libraries"
   project "mongoose"
      kind "StaticLib"

      language "C"

      defines { 
         "_CRT_SECURE_NO_WARNINGS", 
         "_WINSOCK_DEPRECATED_NO_WARNINGS" 
      }

      removeincludedirs "*" 
      includedirs "deps/mongoose" 
                  
      files "deps/mongoose/mongoose.c"


   project "jsoncpp"
      kind "StaticLib"

      defines "_CRT_SECURE_NO_WARNINGS"

      removeincludedirs "*"
      includedirs "deps/jsoncpp" 
                  
      files "deps/jsoncpp/jsoncpp.cpp"

   project "http-parser"
      kind "StaticLib"

      language "C"

      removeincludedirs "*"
      includedirs "deps/http-parser" 
                  
      files "deps/http-parser/http_parser.c"

            
   project "orion"
      kind "SharedLib"

      defines { "ORION_SHARED_EXPORTS" }

      filter "system:Windows"
         links { 
            "mincore", 
            "Advapi32", 
            "psapi", 
            "ntdll", 
            "rpcrt4" 
         }

      files { 
         "lib/*.cpp",
         "lib/log/*.cpp", 
         "lib/unittest/*.cpp" 
      }

      FilterPlatformSourceFiles()
      
      UseBoostLibShared("program_options")

   
   project "orion-fs"
      kind "SharedLib"

      dependson "orion"

      defines { "ORION_SHARED_EXPORTS" }

      links "orion"

      files { 
         "lib/fs/*.cpp" 
      }
      
      FilterPlatformSourceFiles()


   project "orion-net"
      kind "SharedLib"

      dependson { 
         "jsoncpp", 
         "mongoose", 
         "http-parser", 
         "orion" 
      }

      defines { 
         "ORION_SHARED_EXPORTS", 
         "_WIN32_WINNT=0x0501",
         "ASIO_STANDALONE"
         --"BOOST_DATE_TIME_NO_LIB", 
         --"BOOST_REGEX_NO_LIB" 
      }

      links { 
         "jsoncpp", 
         "mongoose", 
         "http-parser", 
         "orion" 
      }

      filter "system:Windows"
         links { "Advapi32", "ws2_32", "psapi", "rpcrt4" }

      files { 
         "lib/net/**.cpp" 
      }

      includedirs { "C:/Tools/pkgs/asio-1.10.8/include" }

      FilterPlatformSourceFiles()


-- Unit tests
group "Tests"
   include "tests/tests"


-- Examples
group "Examples"
   if _OPTIONS.examples then
      include "examples/examples"
   end
