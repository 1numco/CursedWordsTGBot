cmake_minimum_required(VERSION 3.15)

project (tgbot1 CXX)

include_directories( ${PROJECT_SOURCE_DIR} )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)
set(target tgbot1)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)
find_package(GTest REQUIRED)

include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR}${GTEST_INCLUDE_DIRS})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

set(HEADERS
    CursedWordsTGBot/server.h
    Modules/queue.hpp
    Modules/task.h
    Tests/UnitTests/test.h
)
set(SOURCES
    CursedWordsTGBot/server.cpp
    Modules/task.cpp
    Tests/UnitTests/test.cpp

)
add_executable(target ${SOURCES} main.cpp)
target_link_libraries(
    target /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} gtest gmock pthread)
    
target_include_directories(target PRIVATE ./CursedWordsTGBot ./Modules ./Tests/UnitTests)