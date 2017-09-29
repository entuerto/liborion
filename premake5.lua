--
-- premake5.lua
-- liborion configuration file.
-- Copyright (c) 2016 Tomas Palazuelos
--

require "premake/clangcl"
require "premake/asio"
require "premake/boost"
require "premake/utils"
require "premake/ninja"

workspace "liborion"
   
   version {
      major = 0,
      minor = 1,
      patch = 0
   }

   -- We set the location of the files Premake will generate
   location "build"

   language "C++"

   SetupDefaultConfigurations()

   toolset "clang"

   filter { "system:windows",  "platforms:windows" }
      toolset "clangcl"

   --removeconfigurations { "Release" }

   FindBoost(1.64)


group "Libraries"
   project "mongoose"
      kind "StaticLib"

      language "C"

      filter { "system:windows" }
         defines { 
            "_CRT_SECURE_NO_WARNINGS", 
            "_WINSOCK_DEPRECATED_NO_WARNINGS" 
         }

      includedirs "deps/mongoose" 
                  
      files "deps/mongoose/mongoose.c"


   project "jsoncpp"
      kind "StaticLib"

      filter { "system:windows" }
         defines "_CRT_SECURE_NO_WARNINGS"

      includedirs "deps/jsoncpp" 
                  
      files "deps/jsoncpp/jsoncpp.cpp"

   project "http-parser"
      kind "StaticLib"

      language "C"

      includedirs "deps/http-parser" 
                  
      files "deps/http-parser/http_parser.c"

            
   project "orion"
      kind "SharedLib"

      UseAsio()
      UseBoost("boost_program_options-mt")

      defines { 
         "ORION_SHARED_EXPORTS"
      }

      includedirs { 
         "include", 
         "lib", 
         "deps", 
      }

      files { 
         "lib/*.cpp",
         "lib/log/*.cpp", 
         "lib/unittest/*.cpp" 
      }

      FilterPlatformSourceFiles()
      
      filter "system:windows"
         links { 
            "ws2_32",
            "psapi", 
            "ntdll", 
            "rpcrt4" 
         }

      filter { "system:windows", "platforms:windows" }
         links { "mincore" }

      filter { "system:windows", "platforms:mingw64" }
         links { "version" }

   
   project "orion-fs"
      kind "SharedLib"

      dependson "orion"

      defines { "ORION_SHARED_EXPORTS" }

      includedirs { 
         "include", 
         "lib", 
         "deps", 
      }

      links "orion"

      files { 
         "lib/fs/*.cpp" 
      }
      
      FilterPlatformSourceFiles()


   project "orion-net"
      kind "SharedLib"

      UseAsio()

      dependson { 
         "jsoncpp", 
         "mongoose", 
         "http-parser", 
         "orion" 
      }

      defines { 
         "ORION_SHARED_EXPORTS"
      }

      includedirs { 
         "include", 
         "lib", 
         "deps", 
      }

      links { 
         "jsoncpp", 
         "mongoose", 
         "http-parser", 
         "orion" 
      }

      filter "system:windows"
         links { "ws2_32", "psapi", "rpcrt4" }

      files { 
         "lib/net/**.cpp" 
      }

      FilterPlatformSourceFiles()


-- Unit tests
group "Tests"
   include "tests/tests"


-- Examples
group "Examples"
   if _OPTIONS.examples then
      include "examples/examples"
   end
