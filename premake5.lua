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

   includedirs { 
      "include", 
      "lib", 
      "deps", 
   }

   UseAsio()

   FindBoost(1.61)

   filter { "system:windows",  "action:gmake or ninja" }
      toolset "clangcl"


group "Libraries"
   project "mongoose"
      kind "StaticLib"

      language "C"

      removeincludedirs "*" 

      includedirs "deps/mongoose" 
                  
      files "deps/mongoose/mongoose.c"

      filter { "system:windows" }
         defines { 
            "_CRT_SECURE_NO_WARNINGS", 
            "_WINSOCK_DEPRECATED_NO_WARNINGS" 
         }


   project "jsoncpp"
      kind "StaticLib"

      removeincludedirs "*"

      includedirs "deps/jsoncpp" 
                  
      files "deps/jsoncpp/jsoncpp.cpp"

      filter { "system:windows" }
         defines "_CRT_SECURE_NO_WARNINGS"


   project "http-parser"
      kind "StaticLib"

      language "C"

      removeincludedirs "*"

      includedirs "deps/http-parser" 
                  
      files "deps/http-parser/http_parser.c"

            
   project "orion"
      kind "SharedLib"

      defines { 
         "ORION_SHARED_EXPORTS"
      }

      files { 
         "lib/*.cpp",
         "lib/log/*.cpp", 
         "lib/unittest/*.cpp" 
      }

      FilterPlatformSourceFiles()
      
      UseBoostLibShared("program_options")

      filter "system:Windows"
         links { 
            "mincore", 
            "Advapi32", 
            "psapi", 
            "ntdll", 
            "rpcrt4" 
         }

   
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
         "ORION_SHARED_EXPORTS"
      }

      links { 
         "jsoncpp", 
         "mongoose", 
         "http-parser", 
         "orion" 
      }

      files { 
         "lib/net/**.cpp" 
      }

      FilterPlatformSourceFiles()

      filter "system:Windows"
         links { "Advapi32", "ws2_32", "psapi", "rpcrt4" }


-- Unit tests
group "Tests"
   include "tests/tests"


-- Examples
group "Examples"
   if _OPTIONS.examples then
      include "examples/examples"
   end
