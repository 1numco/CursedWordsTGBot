#pragma once

#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <string>
#include "logger.hpp"

class ITask{
public:
    virtual void execute() = 0;
    ~ITask() = default;
};

class CursedWordDetectingTask: public ITask{
public:
    CursedWordDetectingTask(std::string mss, std::string cht, std::string fN, std::string lN, std::int64_t id);
    virtual void execute() override;
    ~CursedWordDetectingTask();
private:
    std::string message;
    std::string chat_title;
    std::string firstName;
    std::string lastName;
    std::int64_t id_m;
};


