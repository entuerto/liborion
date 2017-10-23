--
-- examples.lua
-- liborion examples configuration file.
-- Copyright (c) 2016 Tomas Palazuelos
--

project "mod-example"
   kind "SharedLib"

   dependson "orion"

   defines { "ORION_SHARED_EXPORTS" }

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files { 
      "module-example-lib.cpp" 
   }

   links { "orion" }


project "date-example"
   kind "ConsoleApp"

   dependson "orion"

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files "date-example.cpp"

   links { "orion" }


project "datetime-example"
   kind "ConsoleApp"

   dependson "orion"

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files "datetime-example.cpp"

   links { "orion" }


project "log-example"
   kind "ConsoleApp"

   dependson "orion"

   UseAsio()

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files "log-example.cpp"

   links { "orion" }

   filter "system:windows"
      links { 
         "ws2_32",
         "psapi", 
         "ntdll", 
         "rpcrt4" 
      }


project "module-example"
   kind "ConsoleApp"

   dependson "orion"

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files "module-example.cpp"

   links { "orion" }


project "net-example"
   kind "ConsoleApp"

   dependson "orion"

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files "net-example.cpp"

   links { "orion", "Iphlpapi" }


project "signal-example"
   kind "ConsoleApp"

   dependson "orion"

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files "signal-example.cpp"

   links { "orion" }


project "system-info"
   kind "ConsoleApp"

   dependson "orion"

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files "system-info.cpp"

   links { "orion" }

   
project "hello-http-server"
   kind "ConsoleApp"

   dependson { 
      "orion", 
      "orion-net" 
   }

   UseAsio()

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files "hello-http-server.cpp"

   links { 
      "orion", 
      "orion-net" 
   }

   filter "system:windows"
      links { 
         "ws2_32",
         "psapi", 
         "ntdll", 
         "rpcrt4" 
      }


project "hello-http-client"
   kind "ConsoleApp"

   dependson { 
      "orion", 
      "orion-net" 
   }

   UseAsio()

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files "hello-http-client.cpp"

   links { 
      "orion", 
      "orion-net" 
   }

   filter "system:windows"
      links { 
         "ws2_32",
         "psapi", 
         "ntdll", 
         "rpcrt4" 
      }


project "hello-tcp-server"
   kind "ConsoleApp"

   dependson { 
      "orion", 
      "orion-net" 
   }

   UseAsio()

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files "hello-tcp-server.cpp"

   links { 
      "orion", 
      "orion-net" 
   }

   filter "system:windows"
      links { 
         "ws2_32",
         "psapi", 
         "ntdll", 
         "rpcrt4" 
      }


project "hello-tcp-client"
   kind "ConsoleApp"

   dependson { 
      "orion", 
      "orion-net" 
   }

   UseAsio()

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files "hello-tcp-client.cpp"

   links { 
      "orion", 
      "orion-net" 
   }

   filter "system:windows"
      links { 
         "ws2_32",
         "psapi", 
         "ntdll", 
         "rpcrt4" 
      }


project "calculator-rpc-server"
   kind "ConsoleApp"

   dependson { 
      "orion", 
      "orion-net" 
   }

   UseAsio()

   includedirs { 
      "../include", 
      "../lib", 
      "../deps", 
   }

   files "calculator-rpc-server.cpp"

   links { 
      "orion", 
      "orion-net" 
   }

   filter "system:windows"
      links { 
         "ws2_32",
         "psapi", 
         "ntdll", 
         "rpcrt4" 
      }
