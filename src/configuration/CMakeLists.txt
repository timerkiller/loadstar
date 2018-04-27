set(module_name configuration)
project(${module_name} CXX)

message(STATUS "Configuring ${module_name}")

set(sources
    src/ConfigurationLoader.cpp)

set(headers
    include/configuration/ConfigurationLoader.hpp
)

add_library(${module_name} STATIC ${sources} ${headers})

add_library(cplane::${module_name} ALIAS ${module_name})

target_include_directories(${module_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${module_name}
    PUBLIC
        Boost::boost
)
