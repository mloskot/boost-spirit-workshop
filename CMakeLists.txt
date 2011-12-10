cmake_minimum_required(VERSION 2.8)
project(boost-spirit-workshop)

find_package(Boost 1.44 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif() 
message(STATUS "Using Boost headers from ${Boost_INCLUDE_DIR}")

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D_SCL_SECURE_NO_DEPRECATE)
endif()

add_executable(guid_parser guid_parser.cpp)
add_executable(pdal_expressions pdal_expressions.cpp)
add_executable(calc_utree calc_utree.cpp)