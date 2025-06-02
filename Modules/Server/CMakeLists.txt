
set(target server_bot)

add_library(${target} STATIC server.cpp)

target_include_directories( ${target} PUBLIC . ${TgBot_include_DIR} ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${GTEST_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})

target_compile_definitions(${target} PRIVATE -DHAVE_CURL )

target_link_libraries( ${target} ${TgBot_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES}  queue task logger classificator reactor)

set_target_properties(server_bot PROPERTIES LINKER_LANGUAGE CXX)