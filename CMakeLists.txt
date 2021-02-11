cmake_minimum_required(VERSION 3.0.0)


macro(remove_cxx_flag flag)
  string(REPLACE "${flag}" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
endmacro()

message(${CMAKE_CXX_FLAGS_RELEASE})
remove_cxx_flag("-O3")
message(${CMAKE_CXX_FLAGS_RELEASE})
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -O2")
message(${CMAKE_CXX_FLAGS_RELEASE}) # print "-DNDEBUG"

project(Josh_Sid_ACS VERSION 0.1.0)


SET(CMAKE_C_FLAGS  "${CMAKE_CFLAGS} -march=native")

add_executable(project1_naive Project1/mat_mult_naive.c Project1/helper_functions.c Project1/matrix_multiply.c)
add_executable(project1_simd Project1/mat_mult_simd.c Project1/helper_functions.c Project1/matrix_multiply.c)


target_link_libraries(project1_naive m)
target_link_libraries(project1_simd m)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
