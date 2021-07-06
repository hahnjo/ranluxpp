# SPDX-License-Identifier: LGPL-2.1-or-later

find_package(GTest)
if(NOT GTEST_FOUND)
  message(WARNING "GTest not found, disabling tests")
  return()
endif()

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_mulmod mulmod.cpp)
target_link_libraries(test_mulmod ${GTEST_BOTH_LIBRARIES})
add_test(NAME mulmod COMMAND test_mulmod)

add_executable(test_RanluxppEngine RanluxppEngine.cpp)
target_link_libraries(test_RanluxppEngine RANLUX++ ${GTEST_BOTH_LIBRARIES})
add_test(NAME RanluxppEngine COMMAND test_RanluxppEngine)
