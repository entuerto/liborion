--
-- asio.lua
-- Helper functions.
-- Copyright (c) 2017 Tomas Palazuelos
--

function UseAsio()
   local envPath 

   -- assemble a search path, depending on the platform
   if os.is("windows") then
      envPath = os.getenv("PATH") or ""
   else
      envPath = "/usr:/usr/local"
   end

   asioPath = os.getenv("ASIO_PATH") or envPath

   asioIncl = os.pathsearch("asio.hpp", path.join(asioPath, "include"))
   
   defines { 
      "ASIO_STANDALONE"
    }

   filter "system:Windows"
      defines { 
         "_WIN32_WINNT=0x0603"
      }
      buildoptions { "-Wno-microsoft-enum-value" }

   -- Reset the filter for other settings
   filter { }

   includedirs { asioIncl }
end