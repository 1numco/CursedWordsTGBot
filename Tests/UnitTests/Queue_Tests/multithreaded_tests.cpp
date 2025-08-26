#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <deque>
#include <thread>
#include <atomic>
#include <unordered_set>
#include <mutex>
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
    
    bool operator==(const TestTask& other) const {
        return message_ == other.message_ && name_ == other.name_;
    }
};

namespace std {
    template<>
    struct hash<TestTask> {
        size_t operator()(const TestTask& task) const {
            return hash<string>()(task.message_) ^ hash<string>()(task.name_);
        }
    };
}

class ThreadSafeQueueTest : public ::testing::Test {
protected:
    Queue<TestTask> queue;
};

TEST_F(ThreadSafeQueueTest, MultiThreadedPushTake) {

    constexpr size_t num_operations = 1000;
    constexpr size_t word_size = 5;
    
    std::atomic<int> push_count{0};
    std::atomic<int> take_count{0};
    std::mutex set_mutex;
    std::unordered_set<TestTask> task_set;
    
    auto producer = [&]() {
        for (size_t i = 0; i < num_operations; ++i) {
            auto message = generated_words(word_size);
            auto name = generated_words(word_size);
            {
                std::lock_guard<std::mutex> lock(set_mutex);
                task_set.insert(TestTask(message, name));
                push_count++;
            }
            queue.push(std::make_unique<TestTask>(message, name));
        }
        queue.shutdown();
    };
    
    auto consumer = [&]() {
        while (true) {
            auto task_ptr = queue.take();
            if (!task_ptr) break;
            
            {
                std::lock_guard<std::mutex> lock(set_mutex);
                take_count++;
                ASSERT_TRUE(task_set.erase(*task_ptr));
            }
        }
    };
    
    std::thread producer_thread(producer);
    std::thread consumer_thread(consumer);

    producer_thread.join();
    consumer_thread.join();

    ASSERT_EQ(push_count, take_count);
    ASSERT_TRUE(task_set.empty());
}

TEST_F(ThreadSafeQueueTest, BlockingPreperty) {

    std::atomic<bool> take_completed{false};
    std::unique_ptr<TestTask> taken_task = nullptr;

    std::thread consumer_thread([&]() {
        taken_task = queue.take();
        take_completed = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(take_completed.load());

    queue.push(std::make_unique<TestTask>("test", "task"));

    consumer_thread.join();
    EXPECT_TRUE(take_completed.load());
    ASSERT_NE(taken_task, nullptr);
    EXPECT_EQ(taken_task->message_, "test");
    EXPECT_EQ(taken_task->name_, "task");
}


TEST_F(ThreadSafeQueueTest, PushBlocksWhenFullAndUnblocksAfterTake) {
    constexpr size_t queue_size = 100;
    for (size_t i = 0; i < queue_size; ++i) {
        queue.push(std::make_unique<TestTask>(
            "message_" + std::to_string(i), 
            "name_" + std::to_string(i)
        ));
    }

    std::atomic<bool> push_completed{false};
    
    std::thread producer_thread([&]() {
        queue.push(std::make_unique<TestTask>("extra", "task"));
        push_completed = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(push_completed.load());

    auto task = queue.take();
    ASSERT_NE(task, nullptr);

    producer_thread.join();
    EXPECT_TRUE(push_completed.load());
}

TEST_F(ThreadSafeQueueTest, ComplexMultiThreadedScenario) {
    constexpr size_t word_size = 10;
    constexpr size_t operations_per_thread = 200;
    constexpr size_t num_threads = 10;
    
    std::atomic<int> push_count{0};
    std::atomic<int> take_count{0};
    std::mutex set_mutex;
    std::unordered_set<TestTask> task_set;

    auto producer = [&]() {
        for (size_t i = 0; i < operations_per_thread; i++) {
            auto message = generated_words(word_size);
            auto name = generated_words(word_size);
            
            {
                std::lock_guard<std::mutex> lock(set_mutex);
                task_set.insert(TestTask(message, name));
            }
            
            queue.push(std::make_unique<TestTask>(message, name));
            push_count++;
        }
    };

    auto consumer = [&]() {
        for (size_t i = 0; i < operations_per_thread; i++) {
            auto task = queue.take();
            ASSERT_NE(task, nullptr);
            
            {
                std::lock_guard<std::mutex> lock(set_mutex);
                take_count++;
                ASSERT_TRUE(task_set.erase(*task));
            }
        }
    };

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    for (size_t i = 0; i < num_threads; ++i) {
        producers.emplace_back(producer);
        consumers.emplace_back(consumer);
    }

    for (auto& thread : producers) thread.join();
    for (auto& thread : consumers) thread.join();

    ASSERT_EQ(push_count, take_count);
    ASSERT_TRUE(task_set.empty());
}