--
-- examples.lua
-- liborion examples configuration file.
-- Copyright (c) 2016 Tomas Palazuelos
--

project "mod-example"
   kind "SharedLib"

   dependson "orion"

   defines { "ORION_SHARED_EXPORTS" }

   files { 
      "module-example-lib.cpp" 
   }

   links { "orion" }


project "log-example"
   kind "ConsoleApp"

   dependson "orion"

   files "log-example.cpp"

   links { "orion" }


project "module-example"
   kind "ConsoleApp"

   dependson "orion"

   files "module-example.cpp"

   links { "orion" }


project "net-example"
   kind "ConsoleApp"

   dependson "orion"

   files "net-example.cpp"

   links { "orion", "Iphlpapi" }


project "signal-example"
   kind "ConsoleApp"

   dependson "orion"

   files "signal-example.cpp"

   links { "orion" }


project "system-info"
   kind "ConsoleApp"

   dependson "orion"

   files "system-info.cpp"

   links { "orion" }

   
project "hello-server"
   kind "ConsoleApp"

   dependson { "orion", "orion-net" }

   files "hello-server.cpp"

   links { "jsoncpp", "orion", "orion-net" }


project "hello-client"
   kind "ConsoleApp"

   dependson { "orion", "orion-net" }

   files "hello-client.cpp"

   links { "jsoncpp", "orion", "orion-net" }


project "calculator-rpc-server"
   kind "ConsoleApp"

   dependson { "orion", "orion-net" }

   files "calculator-rpc-server.cpp"

   links { "jsoncpp", "orion", "orion-net" }
