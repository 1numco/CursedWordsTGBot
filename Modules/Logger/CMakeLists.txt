set(target logger)

add_library(${target} STATIC logger.cpp)

target_include_directories(${target} PUBLIC .)