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


class ReactorResultTest : public ::testing::Test{
protected:
    void SetUp() override;
    void TearDown() override;
    void generator();
    void checker();

private:
    size_t count_sending_messages;
    std::shared_ptr<TgBot::Bot> t_bot;
    std::int64_t chat_id;
    std::string sent_message;
    const size_t size_map = 10;
    const size_t limit_time = 6;
};