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
    virtual std::string check() = 0;
    virtual ~IClassificator() = default;
};

class SimpleClassificator final: public IClassificator{
public:
    SimpleClassificator(const std::string& message);
    std::string check() final;
    ~SimpleClassificator();
private:
    std::string message_;
};


class CursedWordsClassificator: public IClassificator{
public:
    CursedWordsClassificator(const std::string& message);
    virtual std::string check() override;
    ~CursedWordsClassificator(); 
private:
    std::string message_;
    std::unique_ptr<ToxicityClassifierClient> ptr_client_;
};