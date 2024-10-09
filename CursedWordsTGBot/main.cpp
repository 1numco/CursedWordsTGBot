#include <stdio.h>
#include <tgbot/tgbot.h>
#include <iostream>
#include <ostream>
#include "server.hpp"
#include "queue.hpp"
#include "task.hpp"
#include <string>


int main(int argc, char *argv[]) {

    std::string token = "7229787403:AAH0DVCx0wUQ-G9lkXYoIllHL0DhmdawEZo";
    std::shared_ptr<Queue<Task>> ptr_queue = std::make_shared<Queue<Task>>();
    Server server(token, ptr_queue);
    server.start();

    return 0;
}

