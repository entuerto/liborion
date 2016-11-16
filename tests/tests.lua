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

   UseBoostLibStatic("program_options")

   files {
      "test-encoding.cpp",
      "test-filesystem.cpp",
      "test-logger.cpp",
      "test-string.cpp",
      "test-units.cpp",
      "unittest/*.cpp",
      "test-main.cpp"
   }


project "test-orion-ws"
   kind "ConsoleApp"

   dependson "orion"

   files {
      "test-jsonrpc.cpp",
      "test-main.cpp"
   }

   links { "jsoncpp", "orion", "orion-net", "orion-ws" }

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
