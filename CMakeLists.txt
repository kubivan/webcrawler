cmake_minimum_required(VERSION 2.8)
set(BOOST_ROOT "~/devtools/boost_1_55_0")
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE (Boost 1.55.0 COMPONENTS
  program_options
  thread date_time
  system
  chrono
  filesystem
  regex
  REQUIRED)

find_package(OpenSSL)
message(STATUS ${OPENSSL_LIBRARIES})
#cppnetlib
set(cppnetlib_DIR "~/devtools/cpp-netlib-0.11.0-final-build/install")
set(CPPNETLIB_INCLUDE_DIRS ${cppnetlib_DIR}/include )
find_library(cppnetlib_uri NAMES cppnetlib-uri PATHS ${cppnetlib_DIR}/lib)
find_library(cppnetlib_client NAMES cppnetlib-client-connections cppnetlib-uri cppnetlib-server-parsers PATHS ${cppnetlib_DIR}/lib)
find_library(cppnetlib_server NAMES cppnetlib-server-parsers PATHS ${cppnetlib_DIR}/lib)
set(CPPNETLIB_LIBRARIES ${cppnetlib_client} ${cppnetlib_uri} ) #${cppnetlib_server})

#gumbo_parser
set(gumbo_DIR "~/devtools/gumbo-parser/install/")
set(gumbo_INCLUDE_DIRS ${gumbo_DIR}/include/)
find_library(gumbo_LIBRARIES gumbo ${gumbo_DIR}/lib)

include_directories (include ${Boost_INCLUDE_DIR} ${CPPNETLIB_INCLUDE_DIRS} ${gumbo_INCLUDE_DIRS})

add_executable(webcrawler src/web_crawler.cpp src/hrefs_search.cpp src/main.cpp)

target_link_libraries( webcrawler ${Boost_LIBRARIES} ${CPPNETLIB_LIBRARIES} ${gumbo_LIBRARIES} ssl crypto)