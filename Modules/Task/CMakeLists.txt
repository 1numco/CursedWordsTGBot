set (target task)

add_library(${target} STATIC task.cpp)
target_include_directories(${target} PUBLIC .)
target_link_libraries(${target} logger classificator reactor)

set_target_properties(task PROPERTIES LINKER_LANGUAGE CXX)