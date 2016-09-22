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
   -- We only use Debug and release
   configurations { "Debug", "Release" }

   filter "kind:SharedLib"
      targetsuffix "-%{wks.version.major}.%{wks.version.minor}"

   -- We now only set settings for the Debug configuration
   filter { "configurations:Debug" }
      defines { "DEBUG" }
      -- We want debug symbols in our debug config
      flags { "Symbols" }

   filter { "configurations:Debug", "kind:*Lib" }
      targetsuffix "-%{wks.version.major}.%{wks.version.minor}-d"

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
end

-- exclude platform specific files from the project
function FilterPlatformSourceFiles()
   -- Exclude files for other operating systems (so they don't get compiled)
   filter { "system:macosx", "files:**-win32.* or files:**-linux.*" }
      flags { "ExcludeFromBuild" }

   filter { "system:windows", "files:**-darwin.* or files:**-linux.*" }
      flags { "ExcludeFromBuild"}

   filter { "system:linux", "files:**-darwin.* or files:**-win32.*" }
      flags { "ExcludeFromBuild"}
    
   filter {} -- clear filter!
end

function FindBoostHeader(hdr)
   local envPath 
   local boostPath = os.getenv("BOOST_PATH") or ""

   -- assemble a search path, depending on the platform
   if os.is("windows") then
      envPath = os.getenv("PATH") or ""
   else
      envPath = "/include:/usr/include:/usr/local/include"
   end

   return os.pathsearch(hdr, envPath, boostPath)
end
