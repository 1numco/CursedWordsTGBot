#!/bin/bash -e

project_directory=$(dirname "$(readlink -f "$0")")

cd $project_directory

mkdir -p build
cd build
# cmake ..
: ${TGBOT_CPP_INSTALL_PREFIX:=""}

# Запуск cmake с использованием переменной BASE_PATH
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./build -DTGBOT_CPP_INSTALL_PREFIX=$TGBOT_CPP_INSTALL_PREFIX
make -j$(nproc)

mkdir -p ../bins/CursedWordsTGBot
mkdir -p ../bins/Tests/UnitTests/Queue_Tests
mkdir -p ../bins/Tests/UnitTests/Parser_Tests
mkdir -p ../bins/Tests/UnitTests/SignalHandler_Tests
mkdir -p ../bins/Tests/FunctionalTests/Test_of_echo_bot

cp ./CursedWordsTGBot/CursedWordsTGBot* ../bins/CursedWordsTGBot/
cp ./Tests/UnitTests/Queue_Tests/queue_tests* ../bins/Tests/UnitTests/Queue_Tests/
cp ./Tests/UnitTests/Parser_Tests/parser_tests* ../bins/Tests/UnitTests/Parser_Tests/
cp ./Tests/UnitTests/SignalHandler_Tests/signalhandler_tests* ../bins/Tests/UnitTests/SignalHandler_Tests/
cp ./Tests/FunctionalTests/Test_of_echo_bot/test_of_echo_bot* ../bins/Tests/FunctionalTests/Test_of_echo_bot/
cp ./Tests/FunctionalTests/Test_of_echo_bot/messages.txt ../bins/Tests/FunctionalTests/Test_of_echo_bot/