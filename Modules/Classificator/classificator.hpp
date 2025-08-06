#pragma once

#include <memory>
#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <string>
#include "logger.hpp"
#include "mutex"
#include <cstdio>  // для popen и pclose
#include <cstdlib>  // для std::stod
#include <chrono>
#include <stdexcept>
#include "client.hpp"


class IClassificator{
public:
    virtual double check() = 0;
    virtual ~IClassificator() = default;
};

class SimpleClassificator final: public IClassificator{
public:
    SimpleClassificator(const std::string& message);
    double check() final;
    ~SimpleClassificator() = default;
private:
    std::string message_;
};


class CursedWordsClassificator: public IClassificator{
public:
    CursedWordsClassificator( std::unique_ptr<IClassifierClient> client, const std::string& message);
    virtual double check() override;
    ~CursedWordsClassificator() = default; 
private:
    std::unique_ptr<IClassifierClient> client_;
    std::string message_;
};

