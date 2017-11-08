#--------------------------------------------------------------------------------------------------
# Lib http-parser
#

# Source files
http_parser_src = files(
   'http-parser/http_parser.c'
   )

http_parser_incl = include_directories('http-parser')

#
# Build static library
#
http_parser = static_library('http-parser', 
                             sources: http_parser_src,
                             include_directories: http_parser_incl,
                             pic: true, 
                             install: false)
#
# Dependency for tests and benchmarks and subproject usage
#
http_parser_dep = declare_dependency(link_with: http_parser)