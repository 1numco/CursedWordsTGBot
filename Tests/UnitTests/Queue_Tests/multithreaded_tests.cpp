#include "multithreaded_tests.hpp"

void ThreadSafeQueueTest::SetUp() {

}

void ThreadSafeQueueTest::TearDown() {

}

TEST_F(ThreadSafeQueueTest, SingleThreadedPushTakeTest) {
    const int size_of_queue = 100;
    Queue<TestTask> queue_(size_of_queue);
    
    const int size_words = 5;
    const int size_operations = 1000;
    std::atomic<int> pushCount{0};
    std::atomic<int> takeCount{0};
    std::mutex set_mutex;
    std::unordered_set<TestTask> t_set;
    
    auto pushTask = [&]() {
        for (int i = 0; i < size_operations; ++i) {
            auto message = generated_words(size_words);
            auto name = generated_words(size_words);
            auto task = std::make_unique<TestTask>(message, name);
            {
                std::lock_guard<std::mutex> lck{set_mutex};
                t_set.insert(TestTask(message, name));
                pushCount++;
            }
            bool pushed = queue_.push(std::move(task));
            if (!pushed) {
                break;
            }
        }
        queue_.shutdown(); 
    };
    
    auto takeTask = [&]() {
        while (true) {
            auto task_ptr = queue_.take();
            if (!task_ptr) break;
            
            {
                std::lock_guard<std::mutex> lck{set_mutex};
                takeCount++;
                ASSERT_TRUE(t_set.erase(*task_ptr));
            }
        }
    };
    
    std::thread producer(pushTask);
    std::thread consumer(takeTask);

    producer.join();
    consumer.join();

    ASSERT_EQ(pushCount, takeCount);

    ASSERT_TRUE(t_set.empty());
}

TEST_F(ThreadSafeQueueTest, TakeBlocksWhenEmptyAndUnblocksAfterPush) {
    const int size_of_queue = 10;
    Queue<TestTask> queue_(size_of_queue);

    std::atomic<bool> take_finished{false};
    std::unique_ptr<TestTask> taken_task = nullptr;

    std::thread taking_thread([&]() {
        taken_task = queue_.take();
        take_finished = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(take_finished.load()) << "Take не должен был завершиться — очередь пустая";

    auto task = std::make_unique<TestTask>("test", "task");
    ASSERT_TRUE(queue_.push(std::move(task)));

    taking_thread.join();
    EXPECT_TRUE(take_finished.load()) << "Take должен завершиться после добавления элемента";
    ASSERT_TRUE(taken_task != nullptr);
    EXPECT_EQ(taken_task->message_, "test");
    EXPECT_EQ(taken_task->name_, "task");
}

TEST_F(ThreadSafeQueueTest, PushReturnsFalseWhenQueueIsFull) {
    const int size_of_queue = 2;
    Queue<TestTask> queue_(size_of_queue);

    for (int i = 0; i < size_of_queue; ++i) {
        auto task = std::make_unique<TestTask>("message_" + std::to_string(i), "name_" + std::to_string(i));
        EXPECT_TRUE(queue_.push(std::move(task)));
    }

    auto extra_task = std::make_unique<TestTask>("extra", "task");
    
    std::atomic<bool> push_result{true};
    std::thread pushing_thread([&]() {
        push_result = queue_.push(std::move(extra_task));
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    auto taken = queue_.take();
    ASSERT_TRUE(taken != nullptr);
    
    pushing_thread.join();
    EXPECT_TRUE(push_result.load()) << "Push должен вернуть true после освобождения места";

}

TEST_F(ThreadSafeQueueTest, FullTest) {
    const int size_of_queue = 77;
    Queue<TestTask> queue_(size_of_queue);
    const int size_words = 10;
    const int size_operations = 200;
    const int numThreads = 10;
    std::atomic<int> pushCount{0};
    std::atomic<int> takeCount{0};
    std::mutex set_mutex;
    std::unordered_set<TestTask> t_set;

    auto pushTask = [&]() {
        for (int i = 0; i < size_operations;) {
            auto message = generated_words(size_words);
            auto name = generated_words(size_words);

            {
                std::lock_guard<std::mutex> lock{set_mutex};
                t_set.insert({message, name});
            }

            auto task = std::make_unique<TestTask>(message, name);
            if (queue_.push(std::move(task))) {
                ++i;
                pushCount++;
            }       
        }
    };
    auto takeTask = [&]() {
        for (int i = 0; i < size_operations;) {
            auto task_ptr = queue_.take(); 
            if (!task_ptr) continue;
            std::lock_guard<std::mutex> lock(set_mutex);
            takeCount++; i++;
            auto it = t_set.find(*task_ptr);
            ASSERT_NE(it, t_set.end()) << "Task not found!";
     
            t_set.erase(it);
        }
    };
    {
        std::jthread pushThreads[numThreads];
        std::jthread takeThreads[numThreads];

        for (int i = 0; i < numThreads; ++i) {
            pushThreads[i] = std::jthread(pushTask);
            takeThreads[i] = std::jthread(takeTask);
        }
    }

    ASSERT_EQ(pushCount, takeCount);
    ASSERT_EQ(t_set.size(), 0);
}