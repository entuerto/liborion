--
-- asio.lua
-- Helper functions.
-- Copyright (c) 2017 Tomas Palazuelos
--

function FindAsio()
   local envPath = "/usr:/usr/local"

   -- assemble a search path, depending on the platform
   filter { "system:windows", "platforms:windows" }
      envPath = os.getenv("PATH") or ""

   asioPath = os.getenv("ASIO_PATH") or envPath

   asioIncl = os.pathsearch("asio.hpp", path.join(asioPath, "include"))
   
end

function UseAsio()
   defines { 
      "ASIO_STANDALONE",
    }

   filter "system:windows"
      defines { 
         "_WIN32_WINNT=0x0603"
      }
      buildoptions { "-Wno-microsoft-enum-value" }

   -- Reset the filter for other settings
   filter { }

   includedirs { asioIncl }
end