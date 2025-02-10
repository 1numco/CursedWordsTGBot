#pragma once

#include <memory>
#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <string>

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