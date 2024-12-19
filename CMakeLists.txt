cmake_minimum_required(VERSION 3.15)

project (CursedWordsTGBot CXX)

set(target CursedWordsTGBot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_subdirectory(CursedWordsTGBot)
add_subdirectory(Modules)
add_subdirectory(Tests)

list(APPEND CMAKE_PREFIX_PATH "/home/ivan/tgbot-cpp/")

find_package(TgBot)

find_package(CURL QUIET)

# if (CURL_FOUND)

include_directories("/home/ivan/tgbot-cpp/include/tgbot/")

link_directories("/home/ivan/tgbot-cpp/include/tgbot/")

target_include_directories( ${target} PUBLIC . /home/ivan/tgbot-cpp/include/tgbot/)