set(target parser)

add_library(${target} SHARED parser.cpp)
target_include_directories(${target} INTERFACE .)