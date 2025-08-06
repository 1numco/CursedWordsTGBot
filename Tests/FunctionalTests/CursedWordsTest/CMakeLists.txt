set(target_running_bot running_bot)
set(target_generator generator)
set(target_checker checker)

include_directories(${PROJECT_SOURCE_DIR}/CursedWordsTGBot)

add_executable(${target_running_bot} 
    running_bot_main.cpp
)
target_include_directories(${target_running_bot} PUBLIC .)
target_link_libraries(${target_running_bot} PUBLIC queue logger server_bot task parser worker signalhandler )

add_executable(${target_generator} 
    generator.cpp
    generator_main.cpp
)
target_include_directories(${target_generator} PUBLIC .)
target_link_libraries(${target_generator} PUBLIC queue logger server_bot task parser worker signalhandler )

add_executable(${target_checker} 
    checker.cpp
    checker_main.cpp
)
target_include_directories(${target_checker} PUBLIC .)
target_link_libraries(${target_checker} PUBLIC gtest queue logger server_bot task parser worker signalhandler )

configure_file(${PROJECT_SOURCE_DIR}/Tests/FunctionalTests/messages.txt ${CMAKE_BINARY_DIR}/Tests/FunctionalTests/messages.txt COPYONLY)