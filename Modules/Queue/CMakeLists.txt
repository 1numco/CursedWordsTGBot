set(target queue)

add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE .)