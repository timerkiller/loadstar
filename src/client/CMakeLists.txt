
set(model_name client)

set(sources ClientMain.cpp)
add_executable(${model_name} ${sources})
target_link_libraries(${model_name} libzmq)