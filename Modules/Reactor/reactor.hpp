#pragma once

#include <tgbot/tgbot.h>
#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <string>
#include "logger.hpp"
#include "tgbot/types/ReplyParameters.h"

class IReactor{
public:
    virtual void react(double classification_result) = 0;
    virtual ~IReactor() = default;
};

class EchoReactor final: public IReactor{
public:
    EchoReactor(std::shared_ptr<TgBot::Bot> bot, const std::string& message, std::int64_t chat_id, std::int32_t messageId);
    void react(double classification_result) final;

    ~EchoReactor();
private:
    std::shared_ptr<TgBot::Bot> bot_;
    std::string message_;
    std::int64_t chat_id_;
    std::int32_t messageId_;
};

class CursedWordsReactor final: public IReactor{
public:
    CursedWordsReactor(std::shared_ptr<TgBot::Bot> bot, const std::string& message, std::int64_t chat_id, std::int32_t messageId);
    void react(double classification_result) final;
private:
    std::shared_ptr<TgBot::Bot> bot_;
    std::string message_;
    std::int64_t chat_id_;
    std::int32_t messageId_;
    static inline double threshold = 0.5;
};