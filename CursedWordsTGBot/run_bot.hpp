#include <stdio.h>
#include <csignal>
#include <tgbot/tgbot.h>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <exception>
#include <boost/algorithm/string.hpp>

#include "logger.hpp"
#include "task.hpp"
#include "queue.hpp"
#include "parser.hpp"
#include "server.hpp"
#include "worker.hpp"
#include "client.hpp"
#include "signalhandler.hpp"

void run_bot(std::string token){
    std::unique_ptr<TgBot::Bot> bot_ = std::make_unique<TgBot::Bot>(token);
    
    Logger::getInstance().setName(bot_->getApi().getMe()->username);
    Logger::getInstance().setLevel(Logger::Levels::Debug);

    auto queue_ = std::make_shared<Queue<ITask>> ();
    

    Server server(std::move(bot_), queue_, std::make_unique<ToxicityClassifierClientFactory>());

    Worker worker(queue_);

    SignalHandler handler({ SIGINT, SIGTERM }, [&](){
            static int count = 0;
            if (!count++) {
                server.terminate();
                worker.terminate();

                Logger::getInstance().logInfo(Logger::Levels::Critical, "Recieved shutdown signal. Stop polling!");
            } else {
                Logger::getInstance().logInfo(Logger::Levels::Fatal, "Recieved second shutdown signal. Exiting!");
                std::exit(EXIT_FAILURE);
            }
        }
    );
    std::thread worker_thread(&Worker::run, &worker);
    server.start();
    worker_thread.join();
}