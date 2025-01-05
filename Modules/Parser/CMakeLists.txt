set(target parser)

add_library(${target} STATIC parser.cpp)
target_include_directories(${target} PUBLIC .)

