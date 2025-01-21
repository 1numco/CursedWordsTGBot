#pragma once

#include <memory>
#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <string>
#include "logger.hpp"

class IClassificator{
public:
    virtual bool check() = 0;
    virtual ~IClassificator() = default;
};

class SimpleClassificator: public IClassificator{
public:
    SimpleClassificator(const std::string& message);
    virtual bool check() override;
    ~SimpleClassificator();
private:
    std::string message_;
};