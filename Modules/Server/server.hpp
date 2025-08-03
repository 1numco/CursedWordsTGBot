#pragma once

#include <cstdlib>
#include <stdio.h> 
#include <deque>
#include <string>
#include <stdio.h>
#include <tgbot/tgbot.h>
#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <signal.h>
#include "queue.hpp"
#include "task.hpp"
#include "logger.hpp"
#include "classificator.hpp"
#include "reactor.hpp"
#include "client.hpp"

class ITask;

class Server{
public:
    Server(std::unique_ptr<TgBot::Bot> ptr_bot, std::shared_ptr<Queue<ITask>> queue, std::unique_ptr<IClassifierFactory> ptr_factory);
    void start();
    void terminate();
    ~Server();

private:
    std::shared_ptr<TgBot::Bot> ptr_bot_;
    std::shared_ptr<Queue<ITask>> queue_;
    std::unique_ptr<IClassifierFactory> classifier_factory_;
    std::atomic<bool> shutdown_requested{false};
};