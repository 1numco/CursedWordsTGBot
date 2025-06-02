set(target reactor)

set(Boost_USE_MULTITHREADED ON)

add_library(${target} STATIC reactor.cpp)

target_include_directories( ${target} PUBLIC . ${TgBot_include_DIR} ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${GTEST_INCLUDE_DIRS})

target_link_libraries( ${target} ${TgBot_LIBRARIES}/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} logger)

set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)