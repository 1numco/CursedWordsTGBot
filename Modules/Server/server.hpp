#pragma once

#include <cstdlib>
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


template <typename T>
class Server{
private:
    TgBot::Bot bot_;
    std::shared_ptr<Queue<T>> queue_;
    std::mutex m_;
    inline static bool shutdown_requested_ = true;
public:

    Server(const std::string& token, std::shared_ptr<Queue<T>> queue): bot_(token), queue_(queue){

        Logger::getInstance().setName(bot_.getApi().getMe()->username);
        Logger::getInstance().setLevel(Logger::Levels::Debug);

        bot_.getEvents().onCommand("start", [&](TgBot::Message::Ptr message) {
            bot_.getApi().sendMessage(message->chat->id, "Hi!");
        });

        bot_.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

            if (StringTools::startsWith(message->text, "/start")) {
                return;
            }
            
            {
                std::lock_guard lg(m_);
                (queue.get())->push(T(message->text, message->chat->title, message->from->firstName, message->from->lastName, message->from->id));
            }
            
            bot_.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
            Logger::getInstance().logInfo(Logger::Levels::Info, message->text);

        });

        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);
    }

    void start(){
        try {

                TgBot::TgLongPoll longPoll(bot_);

                while (shutdown_requested_) {
                    Logger::getInstance().logInfo(Logger::Levels::Info, "Long poll started");
                    longPoll.start();
                }
            } catch (TgBot::TgException& e) {
                Logger::getInstance().logInfo(Logger::Levels::Fatal, e.what());
            }
    }

    static void signal_handler(int signal){
        static size_t count_shutdown = 0;
        if(count_shutdown){
            Logger::getInstance().logInfo(Logger::Levels::Fatal, "CRUSH PROGRAMM!");
            std::exit(EXIT_FAILURE);
        }
        shutdown_requested_ = false;
        count_shutdown++;
        Logger::getInstance().logInfo(Logger::Levels::Critical, "EXIT FROM SERVER!");  
    }

    ~Server(){

    }
};

