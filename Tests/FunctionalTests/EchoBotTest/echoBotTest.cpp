#pragma once

#include <gtest/gtest.h>
#include <memory>
#include <fstream>
#include <string>
#include <unordered_set>
#include <map>
#include <set>
#include <functional>
#include <mutex>
#include <thread>
#include <experimental/random>
#include "task.hpp"
#include "queue.hpp"
#include "logger.hpp"
#include <tgbot/tgbot.h>
#include "run_bot.hpp"
#include <chrono>
#include <csignal>

struct Message {
    std::int32_t  messageId_;
    std::string messageText_;
    Message(std::int32_t messageId, std::string messageText) 
        : messageId_(messageId), messageText_(std::move(messageText)) {}
};

class ReactorResultTest : public ::testing::Test {
protected:
    void SetUp() override {
        const char* token_one_= std::getenv("TELEGRAM_TOKEN_GENERATOR");
        t_bot = std::make_shared<TgBot::Bot>(token_one_);
        count_recieve_messages = 0;
        chat_id_ = -1002432345513;
    }

    void TearDown() override {}

    void generator() {
        const char* filePath = std::getenv("MESSAGES_FILE_PATH");
        if (!filePath) {
            filePath = defaultFilePath_; 
        }
        
        std::ifstream inputFile(filePath);
        if (!inputFile) {
            Logger::getInstance().logInfo(Logger::Levels::Critical, "Не удалось открыть файл!\n");
            return;
        }
        
        std::string line;
        while (std::getline(inputFile, line)) {
            {
                std::lock_guard lg{set_mutex};
                message_container.insert(line);
            }
            t_bot->getApi().sendMessage(chat_id_, line);
        }
    }

    void checker() {
        auto last_change_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds;

        t_bot->getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
            count_recieve_messages++;
            {
                std::lock_guard lg{set_mutex};
                ASSERT_TRUE(message_container.count(message->replyToMessage->text));
            }
            last_change_time = std::chrono::steady_clock::now(); 
        });
        
        try {
            TgBot::TgLongPoll longPoll(*t_bot);
            while (count_recieve_messages <= limit_sent_messages_ && 
                   elapsed_seconds.count() < limit_time_in_sec) {
                longPoll.start();
                elapsed_seconds = std::chrono::steady_clock::now() - last_change_time;
            }
        } catch (const TgBot::TgException& e) {
            Logger::getInstance().logInfo(Logger::Levels::Critical, e.what());
        }
    }

private:
    std::atomic<size_t> count_recieve_messages{0};
    std::shared_ptr<TgBot::Bot> t_bot;
    std::int64_t chat_id_;
    static inline const size_t limit_sent_messages_ = 5;
    static inline const size_t limit_time_in_sec = 10;
    std::set<std::string> message_container;
    std::mutex set_mutex;

    static inline const char* defaultFilePath_ = "./Tests/FunctionalTests/EchoBotTest/messages.txt";
};

TEST_F(ReactorResultTest, FirstTest) {

    const char* token_running_bot = std::getenv("TOKEN_RUNNING_MAIN_BOT");
    std::jthread mainThread([&]() {
        run_bot(token_running_bot); 
    });

    generator();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::raise(SIGINT);
    checker();
}