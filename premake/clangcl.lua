---
-- clangcl.lua
-- Interface for the MS C/C++ compiler.
-- Copyright (c) 2016 Tomas Palazuelos
---


   local p = premake

   p.tools.clangcl = {}
   local clangcl = p.tools.clangcl

   local project = p.project
   local config = p.config


--
-- Returns list of C preprocessor flags for a configuration.
--

   function clangcl.getcppflags(cfg)
      return {}
   end

   function clangcl.getrunpathdirs(cfg, dirs)
      return {}
   end

--
-- Returns list of C compiler flags for a configuration.
--

   clangcl.shared = {
      flags = {
         FatalCompileWarnings = "/WX",
         LinkTimeOptimization = "-flto",
         NoFramePointer = "/Oy",
         OmitDefaultLibrary = "/Zl",
         StaticRuntime  = function(cfg) return iif(config.isDebugBuild(cfg), "/MTd", "/MT") end,
         _StaticRuntime = function(cfg) return iif(config.isDebugBuild(cfg), "/MDd", "/MD") end,
         UndefinedIdentifiers = "-Wundef"
      },
      floatingpoint = {
         Fast = "/fp:fast",
         Strict = "/fp:strict",
      },
      floatingpointexceptions = {
         On  = "/fp:except",
         Off = "/fp:except-",
      },
      functionlevellinking = {
         On = "/Gy",
         Off = "/Gy-",
      },
      callingconvention = {
         Cdecl = "/Gd",
         FastCall = "/Gr",
         StdCall = "/Gz",
         VectorCall = "/Gv",
      },
      intrinsics = {
         On = "/Oi",
      },
      optimize = {
         Off = "/Od",
         On = "/Ot",
         Debug = "/Od",
         Full = "/Ox",
         Size = "/Os",
         Speed = "/Ot",
      },
      vectorextensions = {
         AVX = "/arch:AVX",
         AVX2 = "/arch:AVX2",
         SSE = "/arch:SSE",
         SSE2 = "/arch:SSE2",
         SSE3 = "/arch:SSE2",
         SSSE3 = "/arch:SSE2",
         ["SSE4.1"] = "/arch:SSE2",
      },
      warnings = {
         Extra = "/Wall",
         Off = "/W0",
      },
      stringpooling = {
         On = "/GF",
         Off = "/GF-",
      },
      symbols = {
         On = "/Z7"
      }
   }

   clangcl.cflags = {
      cdialect = {
         ["C89"] = "-std=c89",
         ["C90"] = "-std=c90",
         ["C99"] = "-std=c99",
         ["C11"] = "-std=c11",
      }
   }

   function clangcl.getcflags(cfg)
      local shared = config.mapFlags(cfg, clangcl.shared)
      local cflags = config.mapFlags(cfg, clangcl.cflags)
      local flags = table.join(shared, cflags, clangcl.getwarnings(cfg))
      return flags
   end

   function clangcl.getwarnings(cfg)
      local result = {}
      -- TODO
      for _, disable in ipairs(cfg.disablewarnings) do
         table.insert(result, '/wd"' .. disable .. '"')
      end
      for _, fatal in ipairs(cfg.fatalwarnings) do
         table.insert(result, '/we"' .. fatal .. '"')
      end
      return result
   end


--
-- Returns list of C++ compiler flags for a configuration.
--

   clangcl.cxxflags = {
      exceptionhandling = {
         Default = "/EHsc",
         On = "/EHsc",
         SEH = "/EHa",
      },
      cppdialect = {
         ["C++98"] = "-std=c++98",
         ["C++11"] = "-std=c++11",
         ["C++14"] = "-std=c++14",
         ["C++17"] = "-std=c++1z",
      },
      rtti = {
         On = "/GR",
         Off = "/GR-"
      }
   }

   function clangcl.getcxxflags(cfg)
      local shared = config.mapFlags(cfg, clangcl.shared)
      local cxxflags = config.mapFlags(cfg, clangcl.cxxflags)
      local flags = table.join(shared, cxxflags, clangcl.getwarnings(cfg))
      return flags
   end


--
-- Decorate defines for the clang-cl command line.
--

   clangcl.defines = {
      characterset = {
         Default = { '/D"_UNICODE"', '/D"UNICODE"' },
         MBCS = '/D"_MBCS"',
         Unicode = { '/D"_UNICODE"', '/D"UNICODE"' },
      }
   }

   function clangcl.getdefines(defines, cfg)
      local result

      -- HACK: I need the cfg to tell what the character set defines should be. But
      -- there's lots of legacy code using the old getdefines(defines) signature.
      -- For now, detect one or two arguments and apply the right behavior; will fix
      -- it properly when the I roll out the adapter overhaul
      if cfg and defines then
         result = config.mapFlags(cfg, clangcl.defines)
      else
         result = {}
      end

      for _, define in ipairs(defines) do
         table.insert(result, '/D"' .. define .. '"')
      end

      if cfg and cfg.exceptionhandling == p.OFF then
         table.insert(result, "/D_HAS_EXCEPTIONS=0")
      end
      
      return result
   end

   function clangcl.getundefines(undefines)
      local result = {}
      for _, undefine in ipairs(undefines) do
         table.insert(result, '/U"' .. undefine .. '"')
      end
      return result
   end


--
-- Returns a list of forced include files, decorated for the compiler
-- command line.
--
-- @param cfg
--    The project configuration.
-- @return
--    An array of force include files with the appropriate flags.
--

   function clangcl.getforceincludes(cfg)
      local result = {}

      table.foreachi(cfg.forceincludes, function(value)
         local fn = project.getrelative(cfg.project, value)
         table.insert(result, "/FI" .. premake.quoted(fn))
      end)

      return result
   end



--
-- Decorate include file search paths for the MSVC command line.
--

   function clangcl.getincludedirs(cfg, dirs, sysdirs)
      local result = {}
      dirs = table.join(dirs, sysdirs)
      for _, dir in ipairs(dirs) do
         dir = project.getrelative(cfg.project, dir)
         table.insert(result, '-I' ..  premake.quoted(dir))
      end
      return result
   end


--
-- Return a list of linker flags for a specific configuration.
--

   clangcl.linkerFlags = {
      flags = {
         FatalLinkWarnings = "/WX",
         LinkTimeOptimization = "/GL",
         NoIncrementalLink = "/incremental:no",
         NoManifest = "/manifest:no",
         OmitDefaultLibrary = "/nodefaultlib",
      },
      kind = {
         SharedLib = function(cfg)
            local r = { "/dll" }
            if not cfg.flags.NoImportLib then
               table.insert(r, '/implib:"' .. cfg.linktarget.relpath .. '"')
            end
            return r
         end
      },
      symbols = {
         On = "/debug"
      }
   }

   clangcl.librarianFlags = {
      flags = {
         FatalLinkWarnings = "/wx",
      }
   }

   function clangcl.getldflags(cfg)
      local map = iif(cfg.kind ~= premake.STATICLIB, clangcl.linkerFlags, clangcl.librarianFlags)
      local flags = config.mapFlags(cfg, map)

      -- Ignore default libraries
      for i, ignore in ipairs(cfg.ignoredefaultlibraries) do
         -- Add extension if required
         if not clangcl.getLibraryExtensions()[ignore:match("[^.]+$")] then
            ignore = path.appendextension(ignore, ".lib")
         end
         table.insert(flags, '/nodefaultlib:' .. ignore)
      end

      return flags
   end


--
-- Build a list of additional library directories for a particular
-- project configuration, decorated for the tool command line.
--
-- @param cfg
--    The project configuration.
-- @return
--    An array of decorated additional library directories.
--

   function clangcl.getLibraryDirectories(cfg)
      local flags = {}
      local dirs = table.join(cfg.libdirs, cfg.syslibdirs)
      for i, dir in ipairs(dirs) do
         dir = project.getrelative(cfg.project, dir)
         table.insert(flags, '/libpath:"' .. dir .. '"')
      end
      return flags
   end


--
-- Return a list of valid library extensions
--

   function clangcl.getLibraryExtensions()
      return {
         ["lib"] = true,
         ["obj"] = true,
      }
   end

--
-- Return the list of libraries to link, decorated with flags as needed.
--

   function clangcl.getlinks(cfg)
      local links = {}

      -- If we need sibling projects to be listed explicitly, grab them first
      if not systemonly then
         links = config.getlinks(cfg, "siblings", "fullpath")
      end

      -- Then the system libraries, which come undecorated
      local system = config.getlinks(cfg, "system", "fullpath")
      for i = 1, #system do
         -- Add extension if required
         local link = system[i]
         if not p.tools.clangcl.getLibraryExtensions()[link:match("[^.]+$")] then
            link = path.appendextension(link, ".lib")
         end

         table.insert(links, link)
      end

      return links
   end

--
-- Returns makefile-specific configuration rules.
--

   function clangcl.getmakesettings(cfg)
      return nil
   end


--
-- Retrieves the executable command name for a tool, based on the
-- provided configuration and the operating environment.
--
-- @param cfg
--    The configuration to query.
-- @param tool
--    The tool to fetch, one of "cc" for the C compiler, "cxx" for
--    the C++ compiler, or "ar" for the static linker.
-- @return
--    The executable command name for a tool, or nil if the system's
--    default value should be used.
--

   clangcl.tools = {
      cc  = "clang-cl",
      cxx = "clang-cl",
      ar  = "llvm-lib",
      rc  = "rc",
   }

   function clangcl.gettoolname(cfg, tool)
      return clangcl.tools[tool]
   end
