#pragma once

#include <memory>
#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <string>
#include "logger.hpp"
#include "classificator.hpp"
#include "reactor.hpp"

class ITask{
public:
    virtual void execute() = 0;
    virtual ~ITask() = default;
};


class BaseMessageProcessingBotTask: public ITask{
public:
    BaseMessageProcessingBotTask(std::shared_ptr<IClassificator> classificator, std::shared_ptr<IReactor> react);
    virtual void execute() final;
protected:
    std::shared_ptr<IClassificator> classificator_; 
    std::shared_ptr<IReactor> reactor_; 
};


class CursedWordDetectingTask: public BaseMessageProcessingBotTask{
public:
    CursedWordDetectingTask(std::shared_ptr<IClassificator> classificator, std::shared_ptr<IReactor> react);
};