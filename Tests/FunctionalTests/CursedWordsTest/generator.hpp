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
#include <chrono>
#include <pthread.h>
#include <csignal>
#include <string>

struct Message {
    std::int32_t  messageId_;
    std::string messageText_;
    Message(std::int32_t messageId, std::string messageText) 
        : messageId_(messageId), messageText_(std::move(messageText)) {}
};

class Generator{

public:
    Generator();
    void generator();

private:
    std::atomic<size_t> count_recieve_messages{0};
    std::shared_ptr<TgBot::Bot> t_bot;
    static inline std::int64_t chat_id_;
    const size_t limit_sent_messages_ = 5;
    const size_t limit_time_in_sec = 8;
    std::map<std::string, bool> message_container;
    std::mutex set_mutex;
    const inline static std::string filePath_ = "./bins/Tests/FunctionalTests/messages.txt";
};