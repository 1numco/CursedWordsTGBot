set(target cursedWordsTest)

add_executable(${target} cursedWordsTest.cpp main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/CursedWordsTGBot)
target_include_directories(${target} PUBLIC .)
target_link_libraries(${target} PUBLIC gtest queue server_bot task parser worker signalhandler )

configure_file(${PROJECT_SOURCE_DIR}/Tests/FunctionalTests/CursedWordsTest/messages.txt ${CMAKE_BINARY_DIR}/Tests/FunctionalTests/CursedWordsTest/messages.txt COPYONLY)
