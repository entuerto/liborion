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

   files {
      "test-encoding.cpp",
      "test-logger.cpp",
      "test-string.cpp",
      "test-units.cpp",
      "unittest/*.cpp",
      "test-main.cpp"
   }

   libdirs {
      "%{cfg.linktarget.directory}"
   }

   links { "orion" }

   filter "system:windows"
      links { 
         "ws2_32",
         "psapi", 
         "ntdll", 
         "rpcrt4" 
      }  
   

project "test-orion-rpc"
   kind "ConsoleApp"

   dependson { 
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

   files {
      "test-jsonrpc.cpp",
      "test-main.cpp"
   }

   links { "orion", "orion-net" }


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

   files {
      "test-net.cpp",
      "test-url.cpp",
      "test-main.cpp"
   }

   links { "orion", "orion-net" }

   filter "system:windows"
      links { "ws2_32", "mswsock", "psapi", "rpcrt4" }

