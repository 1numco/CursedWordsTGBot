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
    std::atomic<int> pushCount{0};  // Счётчик количества успешных вставок
    std::atomic<int> takeCount{0};  // Счётчик количества успешных извлечений
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
            if(it != t_set.end())
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
    const int size_of_queue = 70;
    Queue<TestTask> queue_(size_of_queue);
    const int size_words = 5;
    const int size_operations = 1000;
    std::atomic<int> pushCount{0};  // Счётчик количества успешных вставок
    std::mutex set_mutex;
    std::unordered_set<TestTask> t_set;

    auto pushTask = [&]() {
        for (int i = 0; i < size_operations; ++i) {
            auto message = generated_words(size_words);
            auto name = generated_words(size_words);
            auto task = std::make_unique<TestTask>(message, name);
            if((queue_.push(std::move(task)))){
                pushCount++;
                t_set.insert({message, name});
            }      
        }
    };

    std::thread pushThreads{pushTask};

    pushThreads.join();

    ASSERT_EQ(pushCount, size_of_queue);
    ASSERT_EQ(t_set.size(), size_of_queue);
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
            if (it != t_set.end()){
                t_set.erase(it);
            }
        }
    };
    std::thread pushThreads[numThreads];
    std::thread takeThreads[numThreads];
    for (int i = 0; i < numThreads; ++i) {
        pushThreads[i] = std::thread(pushTask);
        takeThreads[i] = std::thread(takeTask);
    }
    for (int i = 0; i < numThreads; ++i) {
        pushThreads[i].join();
        takeThreads[i].join();
    }
    ASSERT_EQ(pushCount, takeCount);
    ASSERT_EQ(t_set.size(), 0);
}

TEST_F(ThreadSafeQueueTest, TestPushAndTakeConcurrent) {
    const int size_of_queue = 30;
    Queue<TestTask> queue_(size_of_queue);

    std::atomic<int> tasks_taken(0);
    std::atomic<int> tasks_pushed(0);
    const size_t size_words = 10;
    const size_t number_of_operations = 200;
    std::mutex set_mutex;
    std::unordered_set<TestTask> t_set;
    auto push_task = [&]() {
        for (size_t i = 0; i < number_of_operations;) {
            auto message = generated_words(size_words);
            auto name = generated_words(size_words);
            std::lock_guard<std::mutex> lock{set_mutex};
            auto task = std::make_unique<TestTask>(message, name);
            if (queue_.push(std::move(task))) {
                tasks_pushed++; i++;
                t_set.insert({message, name});
            }
        }
    };
    auto take_task = [&]() {
        for (size_t i = 0; i < number_of_operations;) {
            std::unique_ptr<TestTask> task_ptr;
            while(!(task_ptr = queue_.take()));
            std::lock_guard<std::mutex> lock{set_mutex};
            tasks_taken++; i++;
            auto it = t_set.find(*task_ptr);
            if (it != t_set.end()){
                t_set.erase(it);
            }
        }
    };
    std::thread t1(push_task);
    std::thread t2(take_task);
    std::thread t3(take_task);
    std::thread t4(push_task);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    EXPECT_EQ(tasks_taken.load(), tasks_pushed.load());
    ASSERT_EQ(t_set.size(), 0);
}

TEST_F(ThreadSafeQueueTest, TestTakeFromEmptyQueue) {
    const int size_of_queue = 90;
    Queue<TestTask> queue_(size_of_queue);
    std::atomic<int> tasks_taken(0);
    const size_t number_of_operations = 10000;
    auto take_task = [&]() {
        for (size_t i = 0; i < number_of_operations; ++i) {
            auto task = queue_.take();
            if (task) {
                tasks_taken++;
            }
        }
    };
    std::thread t(take_task);
    t.join();

    EXPECT_EQ(tasks_taken.load(), 0);
}

TEST_F(ThreadSafeQueueTest, SingleThreadedPushTakeOfSetTest) {
    const int size_of_queue = 10;
    Queue<TestTask> queue_(size_of_queue);
    const int size_words = 5;
    const int number_of_operations = 500;
    std::mutex set_mutex;
    std::unordered_set<TestTask> t_set;

    auto pushTask = [&]() {
        for (int i = 0; i < number_of_operations;) {
            auto message = generated_words(size_words);
            auto name = generated_words(size_words);
            auto task = std::make_unique<TestTask>(message, name);
            std::lock_guard<std::mutex> lock{set_mutex};
            if(queue_.push(std::move(task))){
                ++i;
                t_set.insert({message, name});
            }
        }
    };
    auto takeTask = [&]() {
        for (int i = 0; i < number_of_operations;) {
            std::unique_ptr<TestTask> task_ptr;
            while (!(task_ptr = queue_.take()));
            std::lock_guard<std::mutex> lock{set_mutex};
            auto it = t_set.find(*task_ptr);
            if (it != t_set.end()){
                t_set.erase(it);
            }
            ++i;
        }
    };
    std::thread pushThreads{pushTask};
    std::thread takeThreads{takeTask};

    pushThreads.join();
    takeThreads.join();
    
    ASSERT_EQ(t_set.size(), 0);
}