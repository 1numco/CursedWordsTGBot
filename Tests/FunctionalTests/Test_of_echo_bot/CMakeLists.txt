set(target test_of_echo_bot)

add_executable(${target} test_of_echo_bot.cpp main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/CursedWordsTGBot)
target_include_directories(${target} PUBLIC .)
target_link_libraries(${target} PUBLIC gtest queue server_bot task parser worker signalhandler )

configure_file(${PROJECT_SOURCE_DIR}/Tests/FunctionalTests/Test_of_echo_bot/messages.txt ${CMAKE_BINARY_DIR}/Tests/FunctionalTests/Test_of_echo_bot/messages.txt COPYONLY)

add_test(NAME queue_test COMMAND ${target})