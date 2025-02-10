#!/bin/bash -e

project_directory=$(dirname "$(readlink -f "$0")")

cd $project_directory

mkdir -p build
cd build

: ${TGBOT_CPP_INSTALL_PREFIX:=""}

cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./build -DTGBOT_CPP_INSTALL_PREFIX=$TGBOT_CPP_INSTALL_PREFIX
make -j$(nproc)

mkdir -p ../bins/CursedWordsTGBot
mkdir -p ../bins/Tests/UnitTests/Queue_Tests
mkdir -p ../bins/Tests/UnitTests/Parser_Tests
mkdir -p ../bins/Tests/UnitTests/SignalHandler_Tests
mkdir -p ../bins/Tests/FunctionalTests/EchoBotTest

cp ./CursedWordsTGBot/CursedWordsTGBot* ../bins/CursedWordsTGBot/
cp ./Tests/UnitTests/Queue_Tests/queue_tests* ../bins/Tests/UnitTests/Queue_Tests/
cp ./Tests/UnitTests/Parser_Tests/parser_tests* ../bins/Tests/UnitTests/Parser_Tests/
cp ./Tests/UnitTests/SignalHandler_Tests/signalhandler_tests* ../bins/Tests/UnitTests/SignalHandler_Tests/
cp ./Tests/FunctionalTests/EchoBotTest/echoBotTest* ../bins/Tests/FunctionalTests/EchoBotTest/
cp ./Tests/FunctionalTests/EchoBotTest/messages.txt ../bins/Tests/FunctionalTests/EchoBotTest/