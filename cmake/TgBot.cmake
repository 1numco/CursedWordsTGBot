# cmake/TgBot.cmake

# Проверка, была ли передана переменная BASE_PATH
if(NOT TGBOT_CPP_INSTALL_PREFIX)
    message(STATUS "TGBOT_CPP_INSTALL_PREFIX is not defined. Using default value.")
    set(TGBOT_CPP_INSTALL_PREFIX "/usr/local")  # Указываем путь по умолчанию /usr/local/include 
    
else()
    message(STATUS "Using TGBOT_CPP_INSTALL_PREFIX: ${TGBOT_CPP_INSTALL_PREFIX}")
    set(TgBot_DIR "${TGBOT_CPP_INSTALL_PREFIX}/lib/cmake/TgBot/")

endif()

set(TgBot_include_DIR "${TGBOT_CPP_INSTALL_PREFIX}/include/")
set(TgBot_LIBRARIES "${TGBOT_CPP_INSTALL_PREFIX}/lib//libTgBot.a" ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})

set(TGBOT_CPP_INSTALL_PREFIX_PATH ${TGBOT_CPP_INSTALL_PREFIX} CACHE PATH "TGBOT_CPP_INSTALL_PREFIX for the TgBot project")
set(TgBot_INCLUDE_DIR ${TgBot_include_DIR} CACHE PATH "Include directory for TgBot")
set(TgBot_LIB_DIR ${TgBot_LIBRARIES} CACHE PATH "Library directory for TgBot")