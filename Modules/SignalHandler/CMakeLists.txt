set (target signalhandler)

add_library(${target} STATIC signalhandler.cpp)
target_include_directories(${target} PUBLIC .)
target_link_libraries(${target} logger)