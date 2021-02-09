cmake_minimum_required(VERSION 3.0.0)
project(Josh_Sid_ACS VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(project1_naive Project1/mat_mult_naive.c Project1/helper_functions.c)
add_executable(project1_simd Project1/mat_mult_simd.c Project1/helper_functions.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
