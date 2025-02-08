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
#include <tgbot/tgbot.h>
#include "run_bot.hpp"
#include <chrono>
#include <pthread.h>
#include <csignal>


struct Message{
    std::int32_t  messageId_;
    std::string messageText_;
    Message( std::int32_t messageId,  std::string messageText): messageId_(messageId), messageText_(messageText) {

    }
};

class ReactorResultTest : public ::testing::Test{
protected:
    void SetUp() override;
    void TearDown() override;
    void generator();
    void checker();

private:
    std::atomic<size_t> count_recieve_messages;
    std::shared_ptr<TgBot::Bot> t_bot;
    std::int64_t chat_id_;
    const size_t limit_sent_messages_ = 5;
    const size_t limit_time_in_sec = 10;
    std::set<std::string> message_container;
    std::mutex set_mutex;
};