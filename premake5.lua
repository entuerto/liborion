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

   FindBoost(1.64)


group "Libraries"
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
            "dbghelp",
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
         "http-parser", 
         "orion" 
      }

      filter "system:windows"
         links { 
            "ws2_32", 
            "mswsock", 
            "ntdll", 
            "psapi", 
            "rpcrt4" 
         }

      files { 
         -- "lib/net/**.cpp" 
         "lib/net/Connection.cpp",
         "lib/net/EndPoint.cpp",
         "lib/net/AddressV4.cpp",
         "lib/net/AddressV4-win32.cpp",
         "lib/net/AddressV6.cpp",
         "lib/net/AddressV6-win32.cpp",
         "lib/net/Url.cpp",
         "lib/net/tcp/Server.cpp",
         "lib/net/tcp/ServerImpl.cpp",
         "lib/net/tcp/ServerConnection.cpp",
         "lib/net/tcp/Session.cpp",
         "lib/net/tcp/SessionImpl.cpp",
         "lib/net/tcp/Utils.cpp",
         "lib/net/http/Error.cpp",
         "lib/net/http/Parser.cpp",
         "lib/net/http/Request.cpp",
         "lib/net/http/Response.cpp",
         "lib/net/http/Server.cpp",
         "lib/net/http/ServerImpl.cpp",
         "lib/net/http/ServerConnection.cpp",
         "lib/net/http/Session.cpp",
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
