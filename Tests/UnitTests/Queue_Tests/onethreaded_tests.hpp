#pragma once

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "task.hpp"
#include "queue.hpp"
#include <experimental/random>

inline std::string generated_words(size_t size){
    std::string word = "";
    for(size_t i = 0; i < size; i++){
        char ch = std::experimental::randint(65,122);
        word += ch;
    }
    return word;
}

struct TestTask {
    std::string message_;
    std::string name_;
    TestTask(std::string message, std::string name) : message_(message), name_(name) {};
    ~TestTask(){};
};

class QueueTest : public ::testing::Test{

protected:
    void SetUp() override;
    void TearDown() override;
    Queue<TestTask> queue;
};
