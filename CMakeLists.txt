# SPDX-License-Identifier: LGPL-2.1-or-later

cmake_minimum_required(VERSION 3.0...3.20 FATAL_ERROR)
project(RANLUX++)

add_compile_options(-Wall -Wextra)

add_library(RANLUX++ STATIC RanluxppEngine.cpp)
target_include_directories(RANLUX++ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(RANLUX++ PROPERTIES PUBLIC_HEADER RanluxppEngine.h)

install(TARGETS RANLUX++
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()
