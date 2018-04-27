set(module_name network)
project(${module_name} CXX)

message(STATUS "Configuring ${module_name}")

set(sources
    src/ZmqReceiver.cpp
)

set(headers
    include/ZmqReceiver.hpp
)

add_library(${module_name} STATIC ${sources} ${headers})

add_library(cplane::${module_name} ALIAS ${module_name})

target_include_directories(${module_name} SYSTEM PUBLIC
    ${cppzmq_INCLUDE_DIR}
    ${ZeroMQ_INCLUDE_DIR}
    ${SYSCOM_CONFIG_INCLUDE_DIR}
)

target_include_directories(${module_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

#target_link_libraries(${module_name} PUBLIC
#)

