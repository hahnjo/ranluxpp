# SPDX-License-Identifier: LGPL-2.1-or-later

cmake_minimum_required(VERSION 3.5...3.20 FATAL_ERROR)
project(RANLUX++)

add_compile_options(-Wall -Wextra)

# RANLUX++ generator
add_library(RANLUX++ STATIC RanluxppEngine.cpp)
target_include_directories(RANLUX++ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(RANLUX++ PROPERTIES PUBLIC_HEADER RanluxppEngine.h)

install(TARGETS RANLUX++
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include)

# Compatibility engines for other RANLUX implementations
add_library(RANLUX++compat STATIC RanluxppCompatEngine.cpp)
target_include_directories(RANLUX++compat PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(RANLUX++compat PROPERTIES PUBLIC_HEADER RanluxppCompatEngine.h)

install(TARGETS RANLUX++compat
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include)

# Interface for the C++ standard
option(RANLUXPP_CXX_STANDARD "Build the interface for the C++ standard" OFF)
if(RANLUXPP_CXX_STANDARD)
  add_library(RANLUX++cxx STATIC std_ranluxpp.cpp)
	target_include_directories(RANLUX++cxx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	set_target_properties(RANLUX++cxx PROPERTIES PUBLIC_HEADER std_ranluxpp.h)

  install(TARGETS RANLUX++cxx
		ARCHIVE DESTINATION lib
		PUBLIC_HEADER DESTINATION include)
endif()

# Interface for GNU Scientific Library
option(RANLUXPP_GSL_INTERFACE "Build the interface for GSL" OFF)
if(RANLUXPP_GSL_INTERFACE)
  find_package(GSL REQUIRED)
  add_library(RANLUX++gsl STATIC RanluxppGSL.cpp)
  target_link_libraries(RANLUX++gsl GSL::gsl)

  install(TARGETS RANLUX++gsl ARCHIVE DESTINATION lib)
endif()

# Testing.
include(CTest)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()
