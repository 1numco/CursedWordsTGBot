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
        for (int i = 0; i < size_operations;) {
            auto message = generated_words(size_words);
            auto name = generated_words(size_words);
            auto task = std::make_unique<TestTask>(message, name);
            std::lock_guard<std::mutex> lck{set_mutex};
            if((queue_.push(std::move(task)))){
                i++; pushCount++;
                t_set.insert({message, name});
            }      
        }
    };
    auto takeTask = [&]() {
        for (int i = 0; i < size_operations;) {
            std::unique_ptr<TestTask> task_ptr;
            while (!(task_ptr = queue_.take()));
            takeCount++; ++i;
            std::lock_guard<std::mutex> lck{set_mutex};
            auto it = t_set.find(*task_ptr);
            assert(it != t_set.end());              
            t_set.erase(it);     
        }
    };
    std::thread pushThreads{pushTask};
    std::thread takeThreads{takeTask};

    pushThreads.join();
    takeThreads.join();

    ASSERT_EQ(pushCount, takeCount);
    ASSERT_EQ(t_set.size(), 0);
}

TEST_F(ThreadSafeQueueTest, LimitedSizeOfQueue) {
    const int size_of_queue = 50;
    Queue<TestTask> queue_(size_of_queue);
    const int size_words = 5;
    const int size_operations = 1000;

    auto pushTask = [&]() {
        for (int i = 0; i < size_operations; i++) {
            auto message = generated_words(size_words);
            auto name = generated_words(size_words);
            auto task = std::make_unique<TestTask>(message, name);
            if((queue_.push(std::move(task)))){};       
        }
    };

    {
        std::jthread pushThreads_one(pushTask);
        std::jthread pushThreads_two(pushTask);
        std::jthread pushThreads_three(pushTask);
    }

    ASSERT_LE(queue_.take() ? 1 : 0, size_of_queue) << "Queue exceeded the limit!";
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
            auto task = std::make_unique<TestTask>(message, name);
            std::lock_guard<std::mutex> lock{set_mutex};
            if((queue_.push(std::move(task)))){
                ++i; pushCount++;
                t_set.insert({message, name});
            }       
        }
    };
    auto takeTask = [&]() {
        for (int i = 0; i < size_operations;) {
            std::unique_ptr<TestTask> task_ptr;
            
            while (!(task_ptr = queue_.take()));
            std::lock_guard<std::mutex> lock(set_mutex);
            takeCount++; i++;
            auto it = t_set.find(*task_ptr);
            assert(it != t_set.end());              
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