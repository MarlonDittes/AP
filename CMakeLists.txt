cmake_minimum_required(VERSION 3.0.0)
project(practical VERSION 0.1.0)
set(CMAKE_BUILD_TYPE Release)

find_package(OpenMP REQUIRED)



add_library(my_lib STATIC argtable3.c functions.cpp heap.cpp)

add_executable(practical main.cpp)

if(OpenMP_CXX_FOUND)
    target_link_libraries(practical PRIVATE OpenMP::OpenMP_CXX my_lib)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

#target_link_libraries(practical my_lib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
