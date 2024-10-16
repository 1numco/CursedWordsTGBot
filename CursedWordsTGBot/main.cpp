#include <stdio.h>
#include <tgbot/tgbot.h>
#include <iostream>
#include <ostream>
#include "queue.hpp"
#include "task.hpp"
#include "memory"
#include <string>
#include "server.hpp"

int main(int argc, char *argv[]) {

    std::string token = "7229787403:AAH0DVCx0wUQ-G9lkXYoIllHL0DhmdawEZo";
    
    std::shared_ptr<Queue<Task>> ptr_queue = std::make_shared<Queue<Task>>();

    std::unique_ptr<TgBot::Bot> ptr_bot= std::make_unique<TgBot::Bot>(token);

    Logger::getInstance().setName(ptr_bot->getApi().getMe()->username);
    Logger::getInstance().setLevel(Logger::Levels::Debug);

    Server server(std::move(ptr_bot), ptr_queue);

    server.start();

    return 0;
}

