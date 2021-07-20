# SPDX-License-Identifier: LGPL-2.1-or-later

find_package(GTest)
if(NOT GTEST_FOUND)
  message(WARNING "GTest not found, disabling tests")
  return()
endif()

add_executable(test_mulmod_opt mulmod_opt.cpp)
target_link_libraries(test_mulmod_opt GTest::Main)
add_test(NAME mulmod_opt COMMAND test_mulmod_opt)

add_executable(test_mulmod_noint128 mulmod_noint128.cpp)
target_link_libraries(test_mulmod_noint128 GTest::Main)
add_test(NAME mulmod_noint128 COMMAND test_mulmod_noint128)

add_executable(test_ranlux_lcg ranlux_lcg.cpp)
target_link_libraries(test_ranlux_lcg GTest::Main)
add_test(NAME ranlux_lcg COMMAND test_ranlux_lcg)

add_executable(test_RanluxppCompatEngine RanluxppCompatEngine.cpp)
target_link_libraries(test_RanluxppCompatEngine RANLUX++compat GTest::Main)
add_test(NAME RanluxppCompatEngine COMMAND test_RanluxppCompatEngine)

add_executable(test_RanluxppEngine RanluxppEngine.cpp)
target_link_libraries(test_RanluxppEngine RANLUX++ GTest::Main)
add_test(NAME RanluxppEngine COMMAND test_RanluxppEngine)

if(RANLUXPP_CXX_STANDARD)
  add_executable(test_std_ranluxpp std_ranluxpp.cpp)
  target_link_libraries(test_std_ranluxpp RANLUX++cxx GTest::Main)
  add_test(NAME std_ranluxpp COMMAND test_std_ranluxpp)
endif()

if(RANLUXPP_GSL_INTERFACE)
  add_executable(test_RanluxppGSL RanluxppGSL.cpp)
  target_link_libraries(test_RanluxppGSL RANLUX++gsl GTest::Main)
  add_test(NAME RanluxppGSL COMMAND test_RanluxppGSL)
endif()
