--
-- mmake.lua
-- Define the makefile action(s).
--

	premake.modules.mmake = {}

	local p = premake
	local make = p.modules.mmake
	local project = p.project


---
-- The GNU make action, with support for the new platforms API
---

	newaction {
		trigger         = "mmake",
		shortname       = "Make for clang-cl",
		description     = "Generate GNU makefiles for POSIX, MinGW, and Cygwin",

		valid_kinds     = { "ConsoleApp", "WindowedApp", "StaticLib", "SharedLib" },

		valid_languages = { "C", "C++" },

		valid_tools     = {
			cc     = { "clang", "clang-cl" }
		},

		onWorkspace = function(wks)
			premake.escaper(make.esc)
			premake.generate(wks, make.getmakefilename(wks, false), make.generate_workspace)
		end,

		onProject = function(prj)
			--premake.escaper(make.esc)
			--local makefile = make.getmakefilename(prj, true)
			--premake.generate(prj, makefile, make.cpp.generate)
		end,

		onCleanWorkspace = function(wks)
			--premake.clean.file(wks, make.getmakefilename(wks, false))
		end,

		onCleanProject = function(prj)
			--premake.clean.file(prj, make.getmakefilename(prj, true))
		end
	}

--
-- Write out the default configuration rule for a workspace or project.
--
-- @param target
--    The workspace or project object for which a makefile is being generated.
--

	function make.defaultconfig(target)
		-- find the right configuration iterator function for this object
		local eachconfig = iif(target.project, project.eachconfig, p.workspace.eachconfig)
		local iter = eachconfig(target)

		-- grab the first configuration and write the block
		local cfg = iter()
		if cfg then
			_p('ifndef config')
			_x('  config=%s', cfg.shortname)
			_p('endif')
			_p('')
		end
	end


---
-- Escape a string so it can be written to a makefile.
---

	function make.esc(value)
		result = value:gsub("\\", "\\\\")
		result = result:gsub("\"", "\\\"")
		result = result:gsub(" ", "\\ ")
		result = result:gsub("%(", "\\(")
		result = result:gsub("%)", "\\)")

		-- leave $(...) shell replacement sequences alone
		result = result:gsub("$\\%((.-)\\%)", "$(%1)")
		return result
	end


--
-- Get the makefile file name for a workspace or a project. If this object is the
-- only one writing to a location then I can use "Makefile". If more than one object
-- writes to the same location I use name + ".make" to keep it unique.
--

	function make.getmakefilename(this, searchprjs)
		local count = 0
		for wks in p.global.eachWorkspace() do
			if wks.location == this.location then
				count = count + 1
			end

			if searchprjs then
				for _, prj in ipairs(wks.projects) do
					if prj.location == this.location then
						count = count + 1
					end
				end
			end
		end

		if count == 1 then
			return "Makefile"
		else
			return ".make"
		end
	end


--
-- Output a makefile header.
--
-- @param target
--    The workspace or project object for which the makefile is being generated.
--

	function make.header(target)
		local kind = iif(target.project, "project", "workspace")

		_p('# %s %s makefile autogenerated by Premake', premake.action.current().shortname, kind)
		_p('')

		if kind == "workspace" then
			_p('.NOTPARALLEL:')
			_p('')
		end

		make.defaultconfig(target)

		_p('ifndef verbose')
		_p('  SILENT = @')
		_p('endif')
		_p('')
	end


--
-- Rules for file ops based on the shell type. Can't use defines and $@ because
-- it screws up the escaping of spaces and parethesis (anyone know a fix?)
--

	function make.mkdirRules(dirname)
		_p('%s:', dirname)
		_p('\t@echo Creating %s', dirname)
		_p('ifeq (posix,$(SHELLTYPE))')
		_p('\t$(SILENT) mkdir -p %s', dirname)
		_p('else')
		_p('\t$(SILENT) mkdir $(subst /,\\\\,%s)', dirname)
		_p('endif')
		_p('')
	end


--
-- Format a list of values to be safely written as part of a variable assignment.
--

	function make.list(value)
		if #value > 0 then
			return " " .. table.concat(value, " ")
		else
			return ""
		end
	end


--
-- Convert an arbitrary string (project name) to a make variable name.
--

	function make.tovar(value)
		value = value:gsub("[ -]", "_")
		value = value:gsub("[()]", "")
		return value
	end


---------------------------------------------------------------------------
--
-- Handlers for the individual makefile elements that can be shared
-- between the different language projects.
--
---------------------------------------------------------------------------

	function make.objdir(cfg)
		_x('  OBJDIR = %s', project.getrelative(cfg.project, cfg.objdir))
	end


	function make.objDirRules(prj)
		make.mkdirRules("$(OBJDIR)")
	end


	function make.phonyRules(prj)
		_p('.PHONY: clean prebuild prelink')
		_p('')
	end


	function make.buildCmds(cfg, event)
		_p('  define %sCMDS', event:upper())
		local steps = cfg[event .. "commands"]
		local msg = cfg[event .. "message"]
		if #steps > 0 then
			steps = os.translateCommands(steps)
			msg = msg or string.format("Running %s commands", event)
			_p('\t@echo %s', msg)
			_p('\t%s', table.implode(steps, "", "", "\n\t"))
		end
		_p('  endef')
	end


	function make.preBuildCmds(cfg, toolset)
		make.buildCmds(cfg, "prebuild")
	end


	function make.preBuildRules(prj)
		_p('prebuild:')
		_p('\t$(PREBUILDCMDS)')
		_p('')
	end


	function make.preLinkCmds(cfg, toolset)
		make.buildCmds(cfg, "prelink")
	end


	function make.preLinkRules(prj)
		_p('prelink:')
		_p('\t$(PRELINKCMDS)')
		_p('')
	end


	function make.postBuildCmds(cfg, toolset)
		make.buildCmds(cfg, "postbuild")
	end


	function make.settings(cfg, toolset)
		if #cfg.makesettings > 0 then
			for _, value in ipairs(cfg.makesettings) do
				_p(value)
			end
		end

		local value = toolset.getmakesettings(cfg)
		if value then
			_p(value)
		end
	end


	function make.shellType()
		_p('SHELLTYPE := msdos')
		_p('ifeq (,$(ComSpec)$(COMSPEC))')
		_p('  SHELLTYPE := posix')
		_p('endif')
		_p('ifeq (/bin,$(findstring /bin,$(SHELL)))')
		_p('  SHELLTYPE := posix')
		_p('endif')
		_p('')
	end


	function make.target(cfg)
		_x('  TARGETDIR = %s', project.getrelative(cfg.project, cfg.buildtarget.directory))
		_x('  TARGET = $(TARGETDIR)/%s', cfg.buildtarget.name)
	end


	function make.targetDirRules(prj)
		make.mkdirRules("$(TARGETDIR)")
	end
