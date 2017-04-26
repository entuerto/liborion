--
-- boost.lua
-- Helper functions.
-- Copyright (c) 2017 Tomas Palazuelos
--

function FindBoost(version)
   local envPath 

   -- assemble a search path, depending on the platform
   if os.is("windows") then
      envPath = os.getenv("PATH") or ""
   else
      envPath = "/include:/usr/include:/usr/local/include"
   end

   boostPath = os.getenv("BOOST_PATH") or envPath

   boostIncl = os.pathsearch("boost/config.hpp", path.join(boostPath, "include"))
   boostLib  = path.join(boostPath, "lib")

   includedirs { boostIncl }
   libdirs { boostLib }
end

function UseBoostLibShared(name)
   local libName = "boost_" .. name .. "*"

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

   -- BOOST_ALL_DYN_LINK
   -- BOOST_PROGRAM_OPTIONS_DYN_LINK

   --print("Found Debug: " .. dbgLibName)
   --print("Found Release: " .. relLibName)

   filter { "configurations:Debug" }
      defines { "BOOST_LIB_DIAGNOSTIC", "BOOST_ALL_NO_LIB", "BOOST_ALL_DYN_LINK" }
      links { dbgLibName }

   filter { "configurations:Release" }
      defines { "BOOST_LIB_DIAGNOSTIC", "BOOST_ALL_NO_LIB", "BOOST_ALL_DYN_LINK" }
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
