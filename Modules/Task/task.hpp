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
    ~ITask() = default;
};


class CWTask: public ITask{
public:
    CWTask(std::shared_ptr<IClassificator> classificator, std::shared_ptr<IReactor> react);
    void execute() final;
    ~CWTask();
protected:
    std::shared_ptr<IClassificator> classificator_; 
    std::shared_ptr<IReactor> reactor_; 
};


class CursedWordDetectingTask: public CWTask{
public:
    CursedWordDetectingTask(std::shared_ptr<IClassificator> classificator, std::shared_ptr<IReactor> react);
    ~CursedWordDetectingTask();

};