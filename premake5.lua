--
-- premake5.lua
-- liborion configuration file.
-- Copyright (c) 2016 Tomas Palazuelos
--

require "premake/clangcl"
require "premake/asio"
require "premake/boost"
require "premake/utils"
require "premake/ninja"

workspace "liborion"
   
   version {
      major = 0,
      minor = 1,
      patch = 0
   }

   -- We set the location of the files Premake will generate
   location "build"

   language "C++"

   SetupDefaultConfigurations()

   toolset "clang"

   filter { "system:windows",  "platforms:windows" }
      toolset "clangcl"

   FindBoost(1.64)


group "Libraries"
   project "http-parser"
      kind "StaticLib"

      language "C"

      includedirs "deps/http-parser" 
                  
      files "deps/http-parser/http_parser.c"

            
   project "orion"
      kind "SharedLib"

      UseAsio()
      UseBoost("boost_program_options-mt")

      defines { 
         "ORION_SHARED_EXPORTS"
      }

      includedirs { 
         "include", 
         "lib", 
         "deps", 
      }

      files { 
         -- Include files
         "include/orion/AsyncService.h",
         "include/orion/Chrono.h",
         "include/orion/Datetime.h",
         "include/orion/detail/AsyncTypes.h",
         "include/orion/Encoding.h",
         "include/orion/ErrorMacros.h",
         "include/orion/Exception.h",
         "include/orion/impl/Chrono.ipp",
         "include/orion/impl/Datetime.ipp",
         "include/orion/Memory.h",
         "include/orion/Object.h",
         "include/orion/Orion-Stddefs.h",
         "include/orion/Signal.h",
         "include/orion/SourceLocation.h",
         "include/orion/StringUtils.h",
         "include/orion/System.h",
         "include/orion/Throw.h",
         "include/orion/Units.h",
         "include/orion/Utils.h",
         "include/orion/Uuid.h",

         -- Config files
         "include/orion/Config.h",
         "include/orion/config/Bsd.h",
         "include/orion/config/Cygwin.h",
         "include/orion/config/Linux.h",
         "include/orion/config/Macos.h",
         "include/orion/config/Posix.h",
         "include/orion/config/Windows.h",

         -- Logger files
         "include/orion/Log.h",
         "include/orion/log/BasicLogger.h",
         "include/orion/log/ExceptionRecord.h",
         "include/orion/log/Formatter.h",
         "include/orion/log/Function.h",
         "include/orion/log/impl/BasicLogger.ipp",
         "include/orion/log/impl/Logger.ipp",
         "include/orion/log/Level.h",
         "include/orion/log/Logger.h",
         "include/orion/log/LoggerImpl.h",
         "include/orion/log/LoggerService.h",
         "include/orion/log/Macros.h",
         "include/orion/log/MultilineFormatter.h",
         "include/orion/log/OnelineFormatter.h",
         "include/orion/log/OnelineWithSourceInfoFormatter.h",
         "include/orion/log/OutputHandler.h",
         "include/orion/log/Record.h",
         "include/orion/log/StreamOutputHandler.h",

         -- Unit test files
         "include/orion/Test.h",
         "include/orion/unittest/Test.h",
         "include/orion/unittest/TestMacros.h",
         "include/orion/unittest/TestOptions.h",
         "include/orion/unittest/TestOutput.h",
         "include/orion/unittest/TestResult.h",
         "include/orion/unittest/TestRunner.h",
         "include/orion/unittest/TestStdOutput.h",
         "include/orion/unittest/TestSuite.h",
         
         -- Source files
         "lib/AsyncService.cpp",
         "lib/Encoding.cpp",
         "lib/Object.cpp",
         "lib/StringUtils.cpp",
         "lib/System.cpp",
         -- Logger files
         "lib/log/EndRecord.h", 
         "lib/log/ExceptionRecord.cpp", 
         "lib/log/Function.cpp", 
         "lib/log/Level.cpp", 
         "lib/log/Logger.cpp", 
         "lib/log/LoggerImpl.cpp", 
         "lib/log/MultilineFormatter.cpp", 
         "lib/log/OnelineFormatter.cpp", 
         "lib/log/OnelineWithSourceInfoFormatter.cpp", 
         "lib/log/OutputHandler.cpp", 
         "lib/log/Record.cpp", 
         "lib/log/StartRecord.h", 
         "lib/log/StreamOutputHandler.cpp", 
         "lib/log/SystemInfoRecord.h", 
         -- Unit test files
         "lib/unittest/Test.cpp", 
         "lib/unittest/TestOutput.cpp", 
         "lib/unittest/TestResult.cpp", 
         "lib/unittest/TestRunner.cpp", 
         "lib/unittest/TestStdOutput.cpp", 
         "lib/unittest/TestSuite.cpp"
      }

      filter { "system:windows" }
         files {
            "lib/Module-win32.cpp",
            "lib/System-win32.cpp",
            "lib/Uuid-win32.cpp"
         }

      filter { "system:macosx" }
         files {
            "lib/Module-darwin.cpp",
            "lib/System-darwin.cpp",
            "lib/Uuid-darwin.cpp"
         }

      filter { "system:linux" }
         files {
            "lib/Module-linux.cpp",
            "lib/System-linux.cpp",
            "lib/Uuid-linux.cpp"
         }

      filter {}

      links { "fmt" }

      filter { "system:windows" }
         linkoptions { "-Wl,--allow-multiple-definition" }
         links { 
            "dbghelp",
            "ws2_32",
            "psapi", 
            "ntdll", 
            "rpcrt4" 
         }

      filter { "system:windows", "platforms:windows" }
         links { "mincore" }

      filter { "system:windows", "platforms:mingw64" }
         links { "version" }

      filter {}

   
   project "orion-fs"
      kind "SharedLib"

      dependson "orion"

      defines { "ORION_SHARED_EXPORTS" }

      includedirs { 
         "include", 
         "lib", 
         "deps", 
      }

      links "orion"

      files { 
         "lib/fs/Path.cpp" 
      }
      
      filter { "system:windows" }
         files {
            "lib/fs/utils-win32.cpp"
         }


   project "orion-net"
      kind "SharedLib"

      UseAsio()

      dependson { 
         "http-parser", 
         "orion" 
      }

      defines { 
         "ORION_SHARED_EXPORTS"
      }

      includedirs { 
         "include", 
         "lib", 
         "deps", 
      }

      links { 
         "fmt",
         "http-parser", 
         "orion" 
      }

      filter "system:windows"
         linkoptions { "-Wl,--allow-multiple-definition" }
         links { 
            "ws2_32", 
            "mswsock", 
            "ntdll", 
            "psapi", 
            "rpcrt4" 
         }

      filter {}

      files { 
         -- Include files
         "include/orion/net/Address.h",
         "include/orion/net/AddressV4.h",
         "include/orion/net/AddressV6.h",
         "include/orion/net/Connection.h",
         "include/orion/net/EndPoint.h",
         "include/orion/net/Error.h",
         "include/orion/net/Server.h",
         "include/orion/net/Types.h",
         "include/orion/net/Url.h",
         "include/orion/net/Utils.h",
         "include/orion/net/impl/Connection.ipp",
         -- Http files
         "include/orion/net/http/Client.h",
         "include/orion/net/http/Cookies.h",
         "include/orion/net/http/Error.h",
         "include/orion/net/http/Request.h",
         "include/orion/net/http/Response.h",
         "include/orion/net/http/Server.h",
         "include/orion/net/http/Session.h",
         "include/orion/net/http/StatusCode.h",
         "include/orion/net/http/Utils.h",
         "include/orion/net/http/impl/StatusCode.ipp",
         "include/orion/net/http/impl/Utils.ipp",
         -- RPC files
         "include/orion/net/rpc/Error.h",
         "include/orion/net/rpc/Method.h",
         "include/orion/net/rpc/Params.h",
         "include/orion/net/rpc/Result.h",
         "include/orion/net/rpc/Rpc.h",
         "include/orion/net/rpc/Service.h",
         "include/orion/net/rpc/Traits.h",
         -- TCP files
         "include/orion/net/tcp/Connection.h",
         "include/orion/net/tcp/Server.h",
         "include/orion/net/tcp/Session.h",
         "include/orion/net/tcp/Utils.h",
         "include/orion/net/tcp/impl/Connection.ipp",

         -- Source files 
         "lib/net/Address.cpp",
         "lib/net/AddressV4.cpp",
         "lib/net/AddressV6.cpp",
         "lib/net/EndPoint.cpp",
         "lib/net/Error.cpp",
         "lib/net/Url.cpp",
         -- HTTP files
         "lib/net/http/Error.cpp",
         "lib/net/http/Parser.cpp",
         "lib/net/http/Request.cpp",
         "lib/net/http/Response.cpp",
         "lib/net/http/Server.cpp",
         "lib/net/http/ServerImpl.cpp",
         "lib/net/http/ServerConnection.cpp",
         "lib/net/http/Session.cpp",
         -- TCP files
         "lib/net/tcp/Server.cpp",
         "lib/net/tcp/ServerImpl.cpp",
         "lib/net/tcp/ServerConnection.cpp",
         "lib/net/tcp/Session.cpp",
         "lib/net/tcp/SessionImpl.cpp",
         "lib/net/tcp/Utils.cpp"
         -- RPC files
      }

      filter { "system:windows" }
         files {
            "lib/net/AddressV4-win32.cpp",
            "lib/net/AddressV6-win32.cpp"
         }

      filter { "system:macosx" }
         files {
            "lib/net/AddressV4-darwin.cpp",
            "lib/net/AddressV6-darwin.cpp"
         }

      filter {}


-- Unit tests
group "Tests"
   include "tests/tests"


-- Examples
group "Examples"
   if _OPTIONS.examples then
      include "examples/examples"
   end
