cmake_minimum_required(VERSION 2.8)
set(BOOST_ROOT "D:/devtools/boost_1_55_0")
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

message (STATUS ${BOOST_LIBRARYDIR})

find_package(cppnetlib REQUIRED)
include_directories (${Boost_INCLUDE_DIR})
include_directories (${CPPNETLIB_INCLUDE_DIRS})
include_directories ("D:/devtools/rapidxml-1.13")
message (STATUS ${CPPNETLIB_INCLUDE_DIRS})
add_executable(webcrawler webcrawler.cpp)

message (STATUS ${CPPNETLIB_LIBRARIES})
# message (STATUS ${Boost_LIBRARIES})
target_link_libraries( webcrawler ${Boost_LIBRARIES} ${CPPNETLIB_LIBRARIES})




