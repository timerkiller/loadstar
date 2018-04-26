set(model_name server)
set(sources ServerMain.cpp)
add_executable(${model_name} ${sources})
target_link_libraries(${model_name} libzmq ${Boost_LIBRARIES})