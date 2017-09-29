--
-- boost.lua
-- Helper functions.
-- Copyright (c) 2017 Tomas Palazuelos
--

local boostInclude = ""
local boostLib = ""

function FindBoost(version)
   local envPath = "/include:/usr/include:/usr/local/include"

   -- assemble a search path, depending on the platform
   filter { "system:windows",  "platforms:windows" }
      envPath = os.getenv("PATH") or ""

   boostPath = os.getenv("BOOST_PATH") or envPath

   boostInclude = os.pathsearch("boost/config.hpp", boostPath)
   boostLib  = path.join("C:/Tools/msys64/mingw64", "lib")

end

function UseBoost(name)

   defines { 
      "BOOST_LIB_DIAGNOSTIC", 
      "BOOST_ALL_NO_LIB", 
      "BOOST_ALL_DYN_LINK" 
   }

   links { name }
end

function UseBoostLibShared(name)
   local libName = path.join(boostLib, "*boost_" .. name .. "*")

   local libsFound = os.matchfiles(libName)

   --print(table.tostring(libsFound, 1))

   local dbgLibName = iif(os.findlib(libName), libName, "")
   local relLibName = iif(os.findlib(libName), libName, "")

   --for _, fname in ipairs(libsFound) do
   --   if path.islinkable(fname) and fname:findlast("-gd-") then
   --      dbgLibName = path.getbasename(fname)
   --   end 
   --   if path.islinkable(fname) and not fname:findlast("-gd-") then
   --      relLibName = path.getbasename(fname)
   --   end   
   --end

   filter { "system:windows" }
      dbgLibName = path.getbasename(dbgLibName)
      relLibName = path.getbasename(relLibName)

   -- BOOST_ALL_DYN_LINK
   -- BOOST_PROGRAM_OPTIONS_DYN_LINK

   -- print("Found Debug: " .. dbgLibName)
   -- print("Found Release: " .. relLibName)

   defines { "BOOST_LIB_DIAGNOSTIC", "BOOST_ALL_NO_LIB", "BOOST_ALL_DYN_LINK" }
   includedirs { boostInclude }
   libdirs { boostLib }

   filter { "configurations:debug" }
      links { dbgLibName }

   filter { "configurations:release" }
      links { relLibName }

   filter {}
end

function UseBoostLibStatic(name)
   local libName = "libboost_" .. name .. "*"

   local libsFound = os.matchfiles(path.join(boostLib, libName))
   
   --print(table.tostring(libsFound, 1))

   local dbgLibName = ""
   local relLibName = ""
   for _, fname in ipairs(libsFound) do
      if path.islinkable(fname) and fname:findlast("-gd-") then
         dbgLibName = path.getbasename(fname)
      end 
      if path.islinkable(fname) and not fname:findlast("-gd-") then
         relLibName = path.getbasename(fname)
      end   
   end

   --print("Found Debug: " .. dbgLibName)
   --print("Found Release: " .. relLibName)

   filter { "configurations:Debug" }
      links { dbgLibName }

   filter { "configurations:Release" }
      links { relLibName }

   filter {}
end
