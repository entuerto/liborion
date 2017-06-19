--
-- tests.lua
-- liborion tests configuration file.
-- Copyright (c) 2016 Tomas Palazuelos
--

project "test-orion"
   kind "ConsoleApp"

   dependson "orion"

   defines { "ORION_TEST_LOGGER" }

   links { "orion", "orion-fs"}

   files {
      "test-encoding.cpp",
      "test-logger.cpp",
      "test-string.cpp",
      "test-units.cpp",
      "unittest/*.cpp",
      "test-main.cpp"
   }
   
   UseAsio()
   UseBoostLibShared("program_options")

project "test-orion-rpc"
   kind "ConsoleApp"

   dependson { 
      "jsoncpp", 
      "orion", 
      "orion-net" 
   }

   files {
      "test-jsonrpc.cpp",
      "test-main.cpp"
   }

   links { "jsoncpp", "orion", "orion-net" }

   UseBoostLibStatic("program_options")


project "test-orion-net"
   kind "ConsoleApp"

   dependson "orion"

   files {
      "test-net.cpp",
      "test-url.cpp",
      "test-main.cpp"
   }

   links { "orion", "orion-net" }

   UseBoostLibStatic("program_options")
