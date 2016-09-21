--
-- premake5.lua
-- liborion configuration file.
-- Copyright (c) 2016 Tomas Palazuelos
--

require "premake/clangcl"
require "premake/utils"

workspace "liborion"
   --setVersion(0, 1, 0)
   version "0.1.0"

   -- We set the location of the files Premake will generate
   location "build"

   language "C++"

   filter { "system:windows",  "action:gmake" }
      toolset "clangcl"

   configurations { "Debug", "Release" }

   -- We now only set settings for the Debug configuration
   filter { "configurations:Debug" }
      defines { "DEBUG" }
      -- We want debug symbols in our debug config
      flags { "Symbols" }

   -- We now only set settings for Release
   filter { "configurations:Release" }
      defines { "NDEBUG" }
      -- Release should be optimized
      optimize "On"

   filter "system:Windows"
      buildoptions { "-fms-compatibility-version=19" }

   -- Reset the filter for other settings
   filter { }

   -- Common Compiler flags
   flags { "C++14","NoPCH" }
   rtti "On"

   includedirs { "include", "lib", "deps", "/Tools/boost_1_61_0" }

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

   removeincludedirs "*"
   includedirs "deps/jsoncpp" 
               
   files "deps/jsoncpp/jsoncpp.cpp"

         
project "orion"
   kind "SharedLib"

   defines { "ORION_SHARED_EXPORTS" }

   filter "system:Windows"
      links { "Advapi32", "psapi", "ntdll", "rpcrt4" }

   files { 
      "lib/*.cpp",
      "lib/logging/*.cpp", 
      "lib/unittest/*.cpp" 
   }

   FilterPlatformSourceFiles()


project "orion-plugin"
   kind "SharedLib"

   dependson "orion"

   defines { "ORION_SHARED_EXPORTS" }

   links "orion"

   files { 
      "lib/plugin/*.cpp" 
   }
   
   FilterPlatformSourceFiles()


project "orion-net"
   kind "SharedLib"

   dependson { "mongoose", "orion" }

   defines { "ORION_SHARED_EXPORTS" }

   links { "mongoose", "orion" }

   filter "system:Windows"
      links { "Advapi32", "ws2_32" }

   files { 
      "lib/net/**.cpp" 
   }

   FilterPlatformSourceFiles()


project "orion-ws"
   kind "SharedLib"

   dependson { "jsoncpp", "orion", "orion-net" }

   defines { "ORION_SHARED_EXPORTS" }

   links { "jsoncpp", "orion", "orion-net" }

   filter "system:Windows"
      links { "psapi", "rpcrt4" }

   files { 
      "lib/ws/**.cpp" 
   }

   FilterPlatformSourceFiles()


-- Unit tests
project "test-orion"
   kind "ConsoleApp"

   dependson "orion"

   defines { "ORION_TEST_LOGGER" }

   links "orion"

   files {
      "tests/test-encoding.cpp",
      "tests/test-logger.cpp",
      "tests/test-string.cpp",
      "tests/test-units.cpp",
      "tests/unittest/*.cpp",
      "tests/test-main.cpp"
   }


project "test-orion-ws"
   kind "ConsoleApp"

   dependson "orion"

   files {
      "tests/test-jsonrpc.cpp",
      "tests/test-main.cpp"
   }

   links { "jsoncpp", "orion", "orion-net", "orion-ws" }


project "test-orion-net"
   kind "ConsoleApp"

   dependson "orion"

   files {
      "tests/test-net.cpp",
      "tests/test-main.cpp"
   }

   links { "orion", "orion-net" }


-- Examples
project "module-example"
   kind "SharedLib"

   dependson "orion"

   defines { "ORION_SHARED_EXPORTS" }

   includedirs { "examples" }

   files { 
      "examples/module-example-lib.cpp" 
   }

   links { "orion" }


project "log-example"
   kind "ConsoleApp"

   dependson "orion"

   includedirs { "examples" }

   files "examples/log-example.cpp"

   links { "orion" }


project "module-example"
   kind "ConsoleApp"

   dependson "orion"

   files "examples/module-example.cpp"

   links { "orion" }


project "net-example"
   kind "ConsoleApp"

   dependson "orion"

   files "examples/net-example.cpp"

   links { "orion", "Iphlpapi" }


project "signal-example"
   kind "ConsoleApp"

   dependson "orion"

   files "examples/signal-example.cpp"

   links { "orion" }


project "system-info"
   kind "ConsoleApp"

   dependson "orion"

   files "examples/system-info.cpp"

   links { "orion" }

   
project "hello-server"
   kind "ConsoleApp"

   dependson { "orion", "orion-net" }

   files "examples/hello-server.cpp"

   links { "jsoncpp", "orion", "orion-net" }


project "add-json-rpc-server"
   kind "ConsoleApp"

   dependson { "orion", "orion-net", "orion-ws" }

   files "examples/add-json-rpc-server.cpp"

   links { "jsoncpp", "orion", "orion-net", "orion-ws" }
