cmake_minimum_required(VERSION 3.0.0)
project(practical VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(my_lib STATIC argtable3.c functions.cpp heap.cpp arcflag.cpp )

add_executable(practical main.cpp)
target_link_libraries(practical my_lib )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
