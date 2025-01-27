set(target CursedWordsTGBot)

add_executable(${target} main.cpp)

target_include_directories( ${target} PUBLIC . )

target_link_libraries(${target} PUBLIC server_bot logger parser worker queue signalhandler)