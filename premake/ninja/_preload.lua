--
-- Name:        premake-ninja/_preload.lua
-- Purpose:     Define the ninja action.
-- Author:      Dmitry Ivanov
-- Created:     2015/07/04
-- Copyright:   (c) 2015 Dmitry Ivanov
--

local p = premake
local solution = premake.solution
local project = premake.project

newoption
{
   trigger     = "config",
   value       = "VALUE",
   description = "Build configuration to generate; default is 'debug'",
   default     = "debug"
}

newaction
{
   -- Metadata for the command line and help system
   trigger        = "ninja",
   shortname      = "ninja",
   description    = "Ninja is a small build system with a focus on speed",
   module         = "ninja",

   -- The capabilities of this action
   valid_kinds      = {"ConsoleApp", "WindowedApp", "SharedLib", "StaticLib"}, 
   valid_languages  = {"C", "C++"},
   valid_tools      = {cc = { "gcc", "clang", "clangcl", "msc" }},

   -- Workspace and project generation logic
   onWorkspace = function(sln)
      p.eol("\r\n")
      p.indent("  ")
      p.escaper(p.modules.ninja.esc)
      p.generate(sln, "build.ninja", p.modules.ninja.generateWorkspace)
      p.generate(sln, "rules.ninja", p.modules.ninja.generateDefaultRules)
   end,
   onProject = function(prj)
      p.eol("\r\n")
      p.indent("  ")
      p.escaper(p.modules.ninja.esc)
      p.modules.ninja.generateProject(prj)
   end,
   onCleanWorkspace = function(sln)
      -- TODO
   end,
   onCleanProject = function(prj)
      -- TODO
   end,
}


--
-- Decide when the full module should be loaded.
--

return function(cfg)
   return (_ACTION == "ninja")
end
