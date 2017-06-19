--
-- utils.lua
-- Helper functions.
-- Copyright (c) 2016 Tomas Palazuelos
--

local p = premake
local api = p.api

--
-- MAJOR version when you make incompatible API changes,
-- MINOR version when you add functionality in a backwards-compatible manner, and
-- PATCH version when you make backwards-compatible bug fixes.
--
api.register {
   name  = "version",
   scope = "config",
   kind  = "table",
   tokens = true,
}

newoption
{
   trigger     = "examples",
   description = "Compile the examples sources."
}

-- Set the default configuration for the workspace
function SetupDefaultConfigurations()
   -- We only use debug and release
   configurations { "debug", "release" }

   -- We only support 64bit
   architecture "x86_64"

   filter "kind:SharedLib"
      targetsuffix "-%{wks.version.major}.%{wks.version.minor}"

   -- We now only set settings for the debug configuration
   filter { "configurations:debug" }
      defines { "DEBUG" }
      -- We want debug symbols in our debug config
      symbols "On"

   filter { "configurations:debug", "kind:*Lib" }
      targetsuffix "-%{wks.version.major}.%{wks.version.minor}-d"

   -- We now only set settings for release
   filter { "configurations:release" }
      defines { "NDEBUG" }
      -- release should be optimized
      optimize "On"

   filter "system:Windows"
      buildoptions { "-fms-compatibility-version=19" }

   -- Reset the filter for other settings
   filter { }

   -- Common Compiler flags
   flags { "C++14","NoPCH" }
   rtti "On"
end

-- exclude platform specific files from the project
function FilterPlatformSourceFiles()
   -- Exclude files for other operating systems (so they don't get compiled)
   filter { "system:macosx" }
      removefiles { "**-win32.*", "**-linux.*" }

   filter { "system:Windows" }
      removefiles { "**-darwin.*", "**-linux.*" }

   filter { "system:linux" }
      removefiles { "**-darwin.*", "**-win32.*" }
    
   filter {} -- clear filter!
end

function UseLib(name)
   filter { "configurations:debug" }
      links { name .. "-d" }

   filter { "configurations:release" }
      links { name }

   filter {}
end
