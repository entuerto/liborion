--
-- gmake.lua
-- Define the clang-cl makefile action(s).
-- Copyright (c) 2016 Tomas Palazuelos
--

   local p = premake

   local make = p.make
   local cpp = p.make.cpp
   local project = p.project
   local config = p.config
   local fileconfig = p.fileconfig

-- This check may be unnecessary as we only 'require' this file from d.lua
-- IFF the action already exists, however this may help if this file is
-- directly required, rather than d.lua itself.
   local gmake = p.action.get( 'gmake' )
   if gmake == nil then
      error( "Failed to locate prequisite action 'gmake'" )
   end

--
-- Override the buildcommand rule to support clang-cl 
--

   p.override(cpp, "buildcommand", function(oldfn, prj, objext, node)
      local iscfile = node and path.iscfile(node.abspath) or false
      local flags = iif(prj.language == "C" or iscfile, '$(CC) $(ALL_CFLAGS)', '$(CXX) $(ALL_CXXFLAGS)')
      _p('\t$(SILENT) %s $(FORCE_INCLUDE) -o "$@"  -c "$<"', flags, objext)
   end)

--
-- Override the linkCmd rule to support clang-cl 
--

   p.override(make, "linkCmd", function(oldfn, cfg, toolset)
      if cfg.kind == premake.STATICLIB then
         _p('  LINKCMD = $(AR) -out:"$@" $(OBJECTS)')
      elseif cfg.kind == premake.UTILITY then
         -- Empty LINKCMD for Utility (only custom build rules)
         _p('  LINKCMD =')
      else
         -- this was $(TARGET) $(LDFLAGS) $(OBJECTS)
         --   but had trouble linking to certain static libs; $(OBJECTS) moved up
         -- $(LDFLAGS) moved to end (http://sourceforge.net/p/premake/patches/107/)
         -- $(LIBS) moved to end (http://sourceforge.net/p/premake/bugs/279/)

         local cc = iif(cfg.language == "C", "CC", "CXX")
         _p('  LINKCMD = $(%s) -o "$@" $(OBJECTS) $(RESOURCES) -link $(ALL_LDFLAGS) $(LDDEPS) $(LIBS)', cc)
      end
   end)