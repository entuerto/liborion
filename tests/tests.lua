--
-- tests.lua
-- liborion tests configuration file.
-- Copyright (c) 2016 Tomas Palazuelos
--

project "test-orion"
   kind "ConsoleApp"

   dependson "orion"

   UseAsio()
   UseBoost("boost_program_options-mt")

   defines { "ORION_TEST_LOGGER" }

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   libdirs {
      "%{cfg.linktarget.directory}"
   }

   links { "orion", "orion-fs"}

   filter "system:windows"
      links { 
         "ws2_32",
         "psapi", 
         "ntdll", 
         "rpcrt4" 
      }

   files {
      "test-encoding.cpp",
      "test-logger.cpp",
      "test-string.cpp",
      "test-units.cpp",
      "unittest/*.cpp",
      "test-main.cpp"
   }
   

project "test-orion-rpc"
   kind "ConsoleApp"

   dependson { 
      "jsoncpp", 
      "orion", 
      "orion-net" 
   }

   UseAsio()
   UseBoost("boost_program_options-mt")

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   links { "jsoncpp", "orion", "orion-net" }

   files {
      "test-jsonrpc.cpp",
      "test-main.cpp"
   }


project "test-orion-net"
   kind "ConsoleApp"

   dependson "orion"

   UseAsio()
   UseBoost("boost_program_options-mt")

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   links { "orion", "orion-net" }

   filter "system:windows"
      links { "ws2_32", "mswsock", "psapi", "rpcrt4" }

   files {
      "test-net.cpp",
      "test-url.cpp",
      "test-main.cpp"
   }
