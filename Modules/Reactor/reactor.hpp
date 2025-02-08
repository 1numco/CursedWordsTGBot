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
    virtual void react(bool) = 0;
    virtual ~IReactor() = default;
};


class EchoReactor final: public IReactor{
public:
    EchoReactor(std::shared_ptr<TgBot::Bot> ptr_bot, std::string message, std::int64_t chat_id, std::int32_t messageId);
    void react(bool classification_result) final;
    ~EchoReactor();
private:
    std::shared_ptr<TgBot::Bot> ptr_bot_;
    std::string message_;
    std::int64_t chat_id_;
    std::int32_t messageId_;
};