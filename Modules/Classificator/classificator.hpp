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
    virtual bool check() = 0;
    virtual ~IClassificator() = default;
};

class SimpleClassificator final: public IClassificator{
public:
    SimpleClassificator(const std::string& message);
    bool check() final;
    ~SimpleClassificator();
private:
    std::string message_;
};


class CursedWordsClassificator: public IClassificator{
public:
    CursedWordsClassificator( std::unique_ptr<IClassifierClient> ptr_client, const std::string& message);
    virtual bool check() override;
    ~CursedWordsClassificator(); 
private:
    std::unique_ptr<IClassifierClient> ptr_client_;
    std::string message_;
    static float cursedwords;
};

