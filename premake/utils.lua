--
-- utils.lua
-- Helper functions.
-- Copyright (c) 2016 Tomas Palazuelos
--

local p = premake
local api = p.api

api.register {
   name  = "version",
   scope = "config",
   kind  = "string",
   tokens = true,
}

-- MAJOR version when you make incompatible API changes,
-- MINOR version when you add functionality in a backwards-compatible manner, and
-- PATCH version when you make backwards-compatible bug fixes.
function setVersion(major, minor, patch)
   _version = {major, minor, patch}
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
