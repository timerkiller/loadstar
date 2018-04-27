cmake_minimum_required(VERSION 3.0)
project(loadstar CXX)

find_package(ZeroMQ REQUIRED)

#https://cmake.org/cmake/help/v3.0/module/FindBoost.html
find_package(Boost 1.64.0)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

include(${CMAKE_SOURCE_DIR}/cmake/FindZeroMQ.cmake)
#if(ZeroMQ_FOUND)
#    message("-- ZMQ found")
#    include_directories(${ZeroMQ_INCLUDE_DIRS})
#endif()


include_directories(/usr/local/include/)

set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH  ${CMAKE_CURRENT_BINARY_DIR}/lib)

add_compile_options(-std=c++14)
add_subdirectory(src)




