#pragma once

#include <string>
#include <memory>

struct IClassifierClient{
    virtual double ClassifyMessage(const std::string& message) = 0;
};

struct IClassifierFactory{
    virtual std::unique_ptr<IClassifierClient> Create() = 0;
};