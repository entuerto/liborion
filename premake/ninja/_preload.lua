--
-- Name:     premake/ninja/_preload.lua
-- Purpose:  Define the ninja action.
--

local p = premake
local project = premake.project

newoption
{
   trigger     = "build-type",
   value       = "VALUE",
   description = "Build configuration to generate; default is 'debug'",
   default     = "debug",
   allowed     = {
      { "debug",   "Builds a debug version" },
      { "release", "Builds a release version" },
   }
}

newoption
{
   trigger     = "platform",
   value       = "VALUE",
   description = "Platform configuration to generate",
   allowed     = {
      { "mac",     "Apple Mac OS X" },
      { "mingw64", "Mingw64 for windows platform" },
      { "unix",    "Unix and linux platforms" },
      { "windows", "Microsoft Windows" },
   },
} 

newaction
{
   -- Metadata for the command line and help system
   trigger        = "ninja",
   shortname      = "Ninja build system",
   description    = "Ninja is a small build system with a focus on speed",

   -- The capabilities of this action
   valid_kinds      = { "ConsoleApp", "WindowedApp", "SharedLib", "StaticLib" }, 
   valid_languages  = { "C", "C++" },
   valid_tools      = {
      cc = { "gcc", "clang", "clangcl", "msc" }
   },

   -- Workspace and project generation logic
   onWorkspace = function(wks)
      --p.eol("\r\n")
      --p.indent("  ")
      p.escaper(p.modules.ninja.esc)
      p.generate(wks, "build.ninja", p.modules.ninja.generateWorkspace)
      p.generate(wks, "rules.ninja", p.modules.ninja.generateDefaultRules)
   end,

   onProject = function(prj)
      p.eol("\r\n")
      p.indent("  ")
      p.escaper(p.modules.ninja.esc)
      p.generate(prj, p.modules.ninja.getBuildFilename(prj), p.modules.ninja.generateProject)
   end,

   onCleanWorkspace = function(wks)
      p.clean.file(wks, "build.ninja")
      p.clean.file(wks, "rules.ninja")
   end,

   onCleanProject = function(prj)
      p.clean.file(prj, p.modules.ninja.getBuildFilename(prj))
   end,
}

filter { "system:windows", "platforms:mingw64", "kind:SharedLib" }
   targetprefix "lib"
   targetextension ".dll"
   implibextension ".dll.a"

filter { "system:windows", "platforms:mingw64", "kind:StaticLib" }
   targetprefix "lib"
   targetextension ".a"

--
-- Decide when the full module should be loaded.
--

return function(cfg)
   return (_ACTION == "ninja")
end
