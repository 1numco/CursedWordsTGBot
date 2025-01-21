set(target classificator)

add_library(${target} STATIC classificator.cpp)

target_include_directories( ${target} PUBLIC . )

target_link_libraries(${target} PUBLIC logger)