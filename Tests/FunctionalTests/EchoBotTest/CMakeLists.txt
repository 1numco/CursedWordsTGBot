set(target echoBotTest)

add_executable(${target} echoBotTest.cpp main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/CursedWordsTGBot)
target_include_directories(${target} PUBLIC .)
target_link_libraries(${target} PUBLIC gtest queue server_bot task parser worker signalhandler )

configure_file(${PROJECT_SOURCE_DIR}/Tests/FunctionalTests/EchoBotTest/messages.txt ${CMAKE_BINARY_DIR}/Tests/FunctionalTests/EchoBotTest/messages.txt COPYONLY)

add_test(NAME queue_test COMMAND ${target})