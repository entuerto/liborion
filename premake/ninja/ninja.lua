--
-- Name:    premake/ninja/ninja.lua
-- Purpose: Define the ninja action.
--

local p = premake

local tree       = p.tree
local project    = p.project
local config     = p.config
local fileconfig = p.fileconfig

premake.modules.ninja = {}

local ninja = p.modules.ninja

function ninja.esc(value)
   value = value:gsub("%$", "$$") -- TODO maybe there is better way
   value = value:gsub(":", "$:")
   value = value:gsub("\n", "$\n")
   value = value:gsub(" ", "$ ")
   return value
end

-- in some cases we write file names in rule commands directly
-- so we need to propely escape them
function ninja.shesc(value)
   if type(value) == "table" then
      local result = {}
      local n = #value
      for i = 1, n do
         table.insert(result, ninja.shesc(value[i]))
      end
      return result
   end

   if value:find(" ") then
      return "\"" .. value .. "\""
   end

   return value
end

function ninja.generateDefaultRules(wks)
   p.w("#")
   p.w("# premake5 generated file: DO NOT EDIT!")
   p.w("# Generated by \"Ninja\" Generator")
   p.w("#")
   p.w("")
   p.w("# ------------------------------------------------------------------------------")
   p.w("# Rule for cleaning all built files")
   p.w("# ------------------------------------------------------------------------------")
   p.w("rule CLEAN")
   p.w("   command = ninja.exe -t clean")
   p.w("   description = Cleaning all built files...")
   p.w("")
   p.w("# ------------------------------------------------------------------------------")
   p.w("# Rule for printing all primary targets available")
   p.w("# ------------------------------------------------------------------------------")
   p.w("rule HELP")
   p.w("   command = ninja.exe -t targets all")
   p.w("   description = All primary targets available:")
   p.w("")
end

function ninja.workspaceHeader(wks)
   p.w("#")
   p.w("# premake5 generated file: DO NOT EDIT!")
   p.w("# Generated by \"Ninja\" Generator")
   p.w("#")
   p.w("")
   p.w("# ------------------------------------------------------------------------------")
   p.w("# Workspace:     " .. wks.name)
   p.w("# Configuration: " .. ninja.getBuildType())
   p.w("# Platform:      " .. ninja.getBuildPlatform())
   p.w("# ------------------------------------------------------------------------------")
   p.w("")
end

-- generate workspace that will call ninja for projects
function ninja.generateWorkspace(wks)
   
   ninja.workspaceHeader(wks)

   p.w("# ------------------------------------------------------------------------------")
   p.w("# Include default rules file")
   p.w("# ------------------------------------------------------------------------------")
   p.w("include rules.ninja")
   p.w("")
   p.w("# ------------------------------------------------------------------------------")
   p.w("# Project builds")
   p.w("# ------------------------------------------------------------------------------")

   local cfgs    = {} -- key is concatanated name or variant name, value is string of outputs names
   local key     = ""
   local targets = ""

   for prj in p.workspace.eachproject(wks) do
      local cfg = ninja.getBuildConfig(prj)

      key = prj.name 

      targets = targets .. p.esc(prj.name) .. " "

      -- fill list of output files
      if not cfgs[key] then cfgs[key] = "" end
      --cfgs[key] = p.esc(ninja.outputFilename(cfg)) .. " "
      cfgs[key] = project.getrelative(cfg.project, config.gettargetinfo(cfg).abspath)

      -- include other ninja file
      p.w("subninja " .. p.esc(ninja.projectCfgFilename(cfg, true)))
   end

   p.w("")

   p.w("# ------------------------------------------------------------------------------")
   p.w("# targets")
   p.w("# ------------------------------------------------------------------------------")

   for cfg, outputs in pairs(cfgs) do
      p.w("build " .. p.esc(cfg) .. ": phony " .. outputs)
   end

   ninja.workspacePhonyRule(targets)
   
end

function ninja.workspacePhonyRule(targets)
   p.w("")
   p.w("# ------------------------------------------------------------------------------")
   p.w("# The main all target")
   p.w("# ------------------------------------------------------------------------------")
   p.w("build all: phony " .. targets)
   p.w("")
   p.w("# ------------------------------------------------------------------------------")
   p.w("# Make the all target the default")
   p.w("# ------------------------------------------------------------------------------")
   p.w("default all")
   p.w("")
   p.w("# ------------------------------------------------------------------------------")
   p.w("# Clean all the built files")
   p.w("# ------------------------------------------------------------------------------")
   p.w("build clean: CLEAN")
   p.w("")
   p.w("# ------------------------------------------------------------------------------")
   p.w("# Print all primary targets available")
   p.w("# ------------------------------------------------------------------------------")
   p.w("build help: HELP")
   p.w("")
end

function ninja.list(value)
   if #value > 0 then
      return " " .. table.concat(value, " ")
   else
      return ""
   end
end

-- generate all build files for every project configuration
function ninja.generateProject(prj)
   ninja.projectHeader(prj)

   local cfg = ninja.getBuildConfig(prj)
   local toolset = ninja.getToolSet(cfg)

   if toolset.name == "clangcl" then
      ninja.objExt = ".obj"
      ninja.rules = clangcl.rules
   elseif toolset.name == "msc" then
      ninja.objExt = ".obj"
      ninja.rules = msc.rules
   end

   ninja.writeVariables(cfg, toolset)

   ninja.buildRules(cfg)

   -- build all files
   p.w("# ------------------------------------------------------------------------------")
   p.w("# Object build statements")
   p.w("# ------------------------------------------------------------------------------")

   local files = table.shallowcopy(prj._.files)
   local objfiles = {}

   table.foreachi(files, function(node)
      table.insert(objfiles, ninja.buildFiles(cfg, node))
   end)

   -- build final target
   if cfg.kind == p.STATICLIB then
      ninja.buildTargetStaticLib(cfg, objfiles)
   elseif cfg.kind == p.SHAREDLIB then
      ninja.buildTargetSharedLib(cfg, objfiles)
   elseif (cfg.kind == p.CONSOLEAPP) or (cfg.kind == p.WINDOWEDAPP) then
      ninja.buildTargetApp(cfg, objfiles)
   else
      p.error("ninja action doesn't support this kind of target " .. cfg.kind)
   end
   p.w("")
end

function ninja.projectHeader(prj)
   p.w("#")
   p.w("# premake5 generated file: DO NOT EDIT!")
   p.w("# Project build file")
   p.w("# Generated by \"Ninja\" Generator")
   p.w("#")
   p.w("")
   
   -- premake-ninja relies on scoped rules
   -- and they were added in ninja v1.6
   p.w("# ------------------------------------------------------------------------------")
   p.w("# Minimal version of Ninja required by this file")
   p.w("# ------------------------------------------------------------------------------")
   p.w("ninja_required_version = 1.6")
   p.w("")
   p.w("# ------------------------------------------------------------------------------")
   p.w("# Project:       " .. prj.name)
   p.w("# Configuration: " .. ninja.getBuildType())
   p.w("# Platform:      " .. ninja.getBuildPlatform())
   p.w("# ------------------------------------------------------------------------------")
   p.w("")
end

--local function writeRule(cfg, rule)
--   local toolset = ninja.getToolSet(cfg)
--
--   local toolName = toolset.gettoolname(cfg, rule.name)
--
--   if not toolName and rule.name == "link" then
--      toolName = toolset.gettoolname(cfg, iif(cfg.language:lower() == "c++", "cxx", "cc"))
--   end
--
--   local pathVars = {
--      ["toolset.toolname"] = { absolute = false, token = toolName or ""},
--      ["toolset.links"]    = { absolute = false, token = ninja.list(ninja.shesc(toolset.getlinks(cfg, "system"))) },
--   }
--
--   local ctx = p.context.extent(rule, { pathVars = pathVars })
--
--   local buildmessage  = ctx.buildmessage
--   local buildcommands = ctx.buildcommands
--
--   p.w("rule " .. rule.name)
--   p.w("  command = " .. buildcommands[1])
--   p.w("  description = " .. buildmessage)
--
--   print()
--   for k, v in pairs(cfg) do
--      print("key: " .. k .. "  Value: ", v)
--   end
--
--   if cfg.system ~= "windows" or cfg.platform ~= "windows" then
--      p.w("  depfile = $out.d")
--      p.w("  deps = gcc")
--   else
--      p.w("  deps = msvc")
--   end
--
--   p.w("")
--end

local function writeRule(cfg, rule)
   local toolset = ninja.getToolSet(cfg)

   local toolName = toolset.gettoolname(cfg, rule.name)

   if not toolName and rule.name == "link" then
      toolName = toolset.gettoolname(cfg, iif(cfg.language:lower() == "c++", "cxx", "cc"))
   end

   p.w("rule " .. rule.name)
   p.w("  command = " .. string.format(rule.command, toolName))
   p.w("  description = " .. rule.description)

   if rule.depfile then
      p.w("  depfile = " .. rule.depfile)
   end

   if rule.deps then
      p.w("  deps = " .. rule.deps)
   end
   p.w("")
end

function ninja.buildRules(cfg)
   
   p.w("# ------------------------------------------------------------------------------")
   p.w("# Build rules for " .. cfg.name)
   p.w("# ------------------------------------------------------------------------------")

   --local rules = table.join({'cxx', 'cc', 'ar', 'link', 'resource'}, cfg.project.rules)
--
   --for _, name in ipairs(rules) do
   --   local rule = p.global.getRule(name)
   --   writeRule(cfg, rule)
   --end

   for _, rule in pairs(ninja.rules) do
      writeRule(cfg, rule)
   end
end

function ninja.customBuildFile(cfg, node)
   local objfile = ""

   local filecfg = fileconfig.getconfig(node, cfg)

   local rule = p.global.getRuleForFile(node.abspath)
   local environ = table.shallowcopy(filecfg.environ)

   environ.pathVars = {
      ["file.basename"]     = { absolute = false, token = node.basename },
      ["file.abspath"]      = { absolute = true,  token = node.abspath },
      ["file.relpath"]      = { absolute = false, token = node.relpath },
      ["file.name"]         = { absolute = false, token = node.name },
      ["file.objname"]      = { absolute = false, token = node.objname },
      ["file.path"]         = { absolute = true,  token = node.path },
      ["file.directory"]    = { absolute = true,  token = path.getdirectory(node.abspath) },
      ["file.reldirectory"] = { absolute = false, token = path.getdirectory(node.relpath) },
   }

   local shadowContext = p.context.extent(rule, environ)

   local buildoutputs = p.project.getrelative(cfg.project, shadowContext.buildoutputs)
   
   if buildoutputs and #buildoutputs > 0 then
      for _, output in ipairs(buildoutputs) do
         objfile = output

         p.w("build " .. p.esc(output) .. ": " .. rule.name .. " " .. p.esc(node.abspath))
      end
   end

   p.w("")

   return objfile
end

function ninja.buildFiles(cfg, node)
   local objfile = ""

   local filecfg = fileconfig.getconfig(node, cfg)
   if not filecfg or filecfg.flags.ExcludeFromBuild then
      return objfile
   end

   if fileconfig.hasCustomBuildRule(filecfg) then
      return ninja.customBuildFile(cfg, node)
   end

   local rule = ninja.ruleFromFile[node.extension]

   objfile = p.project.getrelative(cfg.project, path.join(cfg.objdir, node.objname .. ninja.objExt))

   p.w("build " .. objfile .. ": " .. rule.name .. " " .. p.esc(node.abspath))

   p.w("")

   return objfile
end

function ninja.buildTargetStaticLib(cfg, objfiles)
   local objfiles = ninja.list(p.esc(objfiles))
   local libs = ninja.list(p.esc(config.getlinks(cfg, "siblings", "fullpath")))

   local target = project.getrelative(cfg.project, config.gettargetinfo(cfg).abspath)

   p.w("# ------------------------------------------------------------------------------")
   p.w("# Link: static lib")
   p.w("# ------------------------------------------------------------------------------")
   p.w(string.format("build %s: ar %s %s", target, objfiles, libs))
end

function ninja.buildTargetSharedLib(cfg, objfiles)
   local objfiles = ninja.list(p.esc(objfiles))
   local libs = ninja.list(p.esc(config.getlinks(cfg, "siblings", "fullpath")))

   local target = project.getrelative(cfg.project, config.gettargetinfo(cfg).abspath)
   local implibname = project.getrelative(cfg.project, config.getlinkinfo(cfg).abspath)

   p.w("# ------------------------------------------------------------------------------")
   p.w("# Link: shared lib")
   p.w("# ------------------------------------------------------------------------------")

   p.w(string.format("build %s: link %s %s", target, objfiles, libs))
   p.w(string.format("build %s: phony %s", implibname, target))
end

function ninja.buildTargetApp(cfg, objfiles)
   local objfiles = ninja.list(p.esc(objfiles))
   local libs = ninja.list(p.esc(config.getlinks(cfg, "siblings", "fullpath")))

   local target = project.getrelative(cfg.project, config.gettargetinfo(cfg).abspath)

   p.w("# ------------------------------------------------------------------------------")
   p.w("# Link: executable")
   p.w("# ------------------------------------------------------------------------------")

   p.w(string.format("build %s: link %s %s", target, objfiles, libs))
end

function ninja.writeVariables(cfg, toolset)
   local buildopt      = ninja.list(cfg.buildoptions)
   local cflags        = ninja.list(toolset.getcflags(cfg))
   local cppflags      = ninja.list(toolset.getcppflags(cfg))
   local cxxflags      = ninja.list(toolset.getcxxflags(cfg))
   local warnings      = ""
   local defines       = ninja.list(table.join(toolset.getdefines(cfg.defines, cfg), toolset.getundefines(cfg.undefines)))
   local includes      = ninja.list(toolset.getincludedirs(cfg, cfg.includedirs, cfg.sysincludedirs))
   local forceincludes = ninja.list(toolset.getforceincludes(cfg)) -- TODO pch
   local ldflags       = ninja.list(table.join(toolset.getLibraryDirectories(cfg), toolset.getrunpathdirs(cfg, cfg.runpathdirs), toolset.getldflags(cfg), cfg.linkoptions))
   local libs          = ninja.list(p.esc(toolset.getlinks(cfg)))
   local lddeps        = ninja.list(p.esc(config.getlinks(cfg, "siblings", "fullpath")))

   p.w("# ------------------------------------------------------------------------------")
   p.w("# Variables")
   p.w("# ------------------------------------------------------------------------------")
   p.w("defines  = " .. defines)
   p.w("includes = " .. includes)
   p.w("cppflags = " .. cppflags .. " $defines $includes")
   p.w("cflags   = " .. buildopt .. cflags .. warnings .. " $cppflags " .. forceincludes)
   p.w("cxxflags = " .. buildopt .. cflags .. cxxflags .. warnings .. " $cppflags " .. forceincludes)
   p.w("resflags = ")
   p.w("")
   p.w("libs     = " .. libs) 
   p.w("lddeps   = " .. lddeps) 
   p.w("ldflags  = " .. ldflags)
   p.w("")
end

-- return name of output binary relative to build folder
function ninja.outputFilename(cfg)
   return project.getrelative(cfg.workspace, cfg.buildtarget.directory) .. "/" .. cfg.buildtarget.name
end

-- return name of build file for configuration
function ninja.projectCfgFilename(cfg, relative)
   if relative ~= nil then
      relative = project.getrelative(cfg.workspace, cfg.location) .. "/"
   else
      relative = ""
   end
   
   local ninjapath = relative .. cfg.project.name
   
   return ninjapath .. ".ninja"
end

function ninja.getBuildFilename(prj)
   return prj.name .. ".ninja"
end

function ninja.getBuildType()
   return _OPTIONS["build-type"]
end

function ninja.getBuildPlatform()
   return _OPTIONS["platform"] or ""
end

function ninja.getBuildConfig(prj)
   return project.findClosestMatch(prj, prj.configurations[ninja.getBuildType()], ninja.getBuildPlatform())
end

function ninja.getToolSet(cfg)
   local default = iif(cfg.system == p.MACOSX, "clang", "gcc")
   local toolset = p.tools[_OPTIONS.cc or cfg.toolset or default]

   if not toolset then
      error("Invalid toolset '" .. cfg.toolset .. "'")
   end

   return toolset
end

-- check if string starts with string
function ninja.startsWith(str, starts)
   return str:sub(0, starts:len()) == starts
end

-- check if string ends with string
function ninja.endsWith(str, ends)
   return str:sub(-ends:len()) == ends
end

-- removes extension from string
function ninja.noext(str, ext)
   return str:sub(0, str:len() - ext:len())
end

local gcc = {}
gcc.rules = {  
   cc = {
      name        = "cc",
      command     = "%s $cflags -MMD -MF $out.d -c -o $out $in",
      description = "cc $out",
      depfile     = "$out.d",
      deps        = "gcc",
   },
   cxx = {
      name        = "cxx",
      command     = "%s $cxxflags -MMD -MF $out.d -c -o $out $in",
      description = "cxx $out",
      depfile     = "$out.d",
      deps        = "gcc",
   },
   ar = {
      name        = "ar",
      command     = "%s rcs $out $in",
      description = "ar $out",
      depfile     = nil,
      deps        = nil,
   },
   link = {
      name        = "link",
      command     = "%s -o $out $in $ldflags $libs",
      description = "link $out",
      depfile     = nil,
      deps        = nil,
   },
   resource = {
      name        = "resource",
      command     = "%s $in -O coff -o $out $resflags",
      description = "rc $out",
      depfile     = nil,
      deps        = nil,
   },
}

local clangcl = {}
clangcl.rules = {  
   cc = {
      name        = "cc",
      command     = "%s $cflags  /showIncludes -c -o $out $in",
      description = "cc $out",
      depfile     = nil,
      deps        = "msvc",
   },
   cxx = {
      name        = "cxx",
      command     = "%s $cxxflags /showIncludes -c -o $out $in",
      description = "cxx $out",
      depfile     = nil,
      deps        = "msvc",
   },
   ar = {
      name        = "ar",
      command     = "%s $in /nologo -OUT:$out",
      description = "ar $out",
      depfile     = nil,
      deps        = nil,
   },
   link = {
      name        = "link",
      command     = "%s -o $out $in -link $ldflags $libs",
      description = "link $out",
      depfile     = nil,
      deps        = nil,
   },
   resource = {
      name        = "resource",
      command     = "%s $in -O coff -o $out $resflags",
      description = "rc $out",
      depfile     = nil,
      deps        = nil,
   },
}

local msc = {}
msc.rules = {  
   cc = {
      name        = "cc",
      command     = "%s $cflags /nologo /showIncludes -c $in /Fo$out",
      description = "cc $out",
      depfile     = nil,
      deps        = "msvc",
   },
   cxx = {
      name        = "cxx",
      command     = "%s $cxxflags /nologo /showIncludes -c $in /Fo$out",
      description = "cxx $out",
      depfile     = nil,
      deps        = "msvc",
   },
   ar = {
      name        = "ar",
      command     = "%s $in /nologo -OUT:$out",
      description = "ar $out",
      depfile     = nil,
      deps        = nil,
   },
   link = {
      name        = "link",
      command     = "%s $in $libs /link $ldflags /nologo /out:$out", 
      description = "link $out",
      depfile     = nil,
      deps        = nil,
   },
   resource = {
      name        = "resource",
      command     = "%s /nologo $resflags /Fo $out $in",
      description = "rc $out",
      depfile     = nil,
      deps        = nil,
   },
}

ninja.objExt = ".o"
ninja.rules = gcc.rules

ninja.ruleFromFile = {
   ['.c']   = ninja.rules.cc,
   ['.cc']  = ninja.rules.cxx,
   ['.cpp'] = ninja.rules.cxx,
   ['.cxx'] = ninja.rules.cxx,
   ['.mm']  = ninja.rules.cxx,
   ['.s']   = ninja.rules.cc,
   ['.m']   = ninja.rules.cc,
   ['.rc']  = ninja.rules.resource,
}

include("_preload.lua")

return ninja
