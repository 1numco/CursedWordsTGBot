#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <deque>
#include <thread>
#include <atomic>
#include "task.hpp"
#include "queue.hpp"
#include <experimental/random>

inline std::string generated_words(size_t size) {
    std::string word;
    word.reserve(size);
    for(size_t i = 0; i < size; i++) {
        char ch = std::experimental::randint(65, 122);
        word += ch;
    }
    return word;
}

struct TestTask {
    std::string message_;
    std::string name_;
    TestTask(std::string message, std::string name) 
        : message_(std::move(message)), name_(std::move(name)) {}
    
    auto operator<=>(const TestTask& other) const  = default;
};

class QueueTest : public ::testing::Test {
protected:
    Queue<TestTask> queue;
};


TEST_F(QueueTest, SingleElement) {
    queue.push(std::make_unique<TestTask>("Hello", "Vlad"));
    auto task_ptr = queue.take();
    EXPECT_EQ(task_ptr->message_, "Hello");
    EXPECT_EQ(task_ptr->name_, "Vlad");
}

TEST_F(QueueTest, FIFO_Order) {
    constexpr size_t count = 100;
    
    for(size_t i = 0; i < count; i++) {
        queue.push(std::make_unique<TestTask>(
            "Msg" + std::to_string(i), 
            "Name" + std::to_string(i)
        ));
    }
    
    for(size_t i = 0; i < count; i++) {
        auto task_ptr = queue.take();
        EXPECT_EQ(task_ptr->message_, "Msg" + std::to_string(i));
        EXPECT_EQ(task_ptr->name_, "Name" + std::to_string(i));
    }
}

TEST_F(QueueTest, ShutdownRejectsNewTasksAndAllowsFinishingCurrent) {
    constexpr size_t count = 5;
    for(size_t i = 0; i < count; i++) {
        queue.push(std::make_unique<TestTask>(
            generated_words(count), generated_words(count)
        ));
    }
    
    queue.shutdown();
    
    EXPECT_THROW(
        queue.push(std::make_unique<TestTask>("Should", "Reject")),
        std::runtime_error
    );
    
    for(size_t i = 0; i < count; i++) {
        auto task = queue.take();
        EXPECT_NE(task, nullptr);
    }
    
    EXPECT_EQ(queue.take(), nullptr);
}