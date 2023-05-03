cmake_minimum_required(VERSION 3.0.0)
project(practical VERSION 0.1.0)

include(CTest)
enable_testing()


add_library(my_lib STATIC argtable3.c)

add_executable(practical practical.cpp)
target_link_libraries(practical PRIVATE my_lib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
