#include "multithreaded_tests.hpp"

void ThreadSafeQueueTest::SetUp() {

}

void ThreadSafeQueueTest::TearDown() {

}

TEST_F(ThreadSafeQueueTest, PushTakeTest) {
    Queue<TestTask> queue_;
    const size_t size_words = 10;
    auto message = generated_words(size_words);
    auto name = generated_words(size_words);
    std::unique_ptr<TestTask> task = std::make_unique<TestTask>(message, name);
    ASSERT_TRUE(queue_.push(std::move(task)));  // Вставляем элемент
    auto result = queue_.take();  // Извлекаем элемент
    ASSERT_NE(result, nullptr);  // Проверяем, что элемент извлечен
    ASSERT_EQ(result->message_, message);
    ASSERT_EQ(result->name_, name);  
}

TEST_F(ThreadSafeQueueTest, SingleThreadedPushTakeTest) {
    Queue<TestTask> queue_(100);
    const int size_words = 5;
    const int size_operations = 100;
    std::atomic<int> pushCount{0};  // Счётчик количества успешных вставок
    std::atomic<int> takeCount{0};  // Счётчик количества успешных извлечений
    auto pushTask = [&]() {
        for (int i = 0; i < size_operations; ++i) {
            auto message = generated_words(size_words);
            auto name = generated_words(size_words);
             auto task = std::make_unique<TestTask>(message, name);
            if((queue_.push(std::move(task))))
                pushCount++;       
        }
    };
    auto takeTask = [&]() {
        for (int i = 0; i < size_operations; ++i) {
            std::unique_ptr<TestTask> task_ptr;
            while (!(task_ptr = queue_.take()));
            takeCount++;     
        }
    };
    std::thread pushThreads{pushTask};
    std::thread takeThreads{takeTask};

    pushThreads.join();
    takeThreads.join();
    
    ASSERT_EQ(pushCount, takeCount);
}

TEST_F(ThreadSafeQueueTest, MultiThreadedPushTakeTest) {
    Queue<TestTask> queue_(500);

    const int size_words = 10;
    const int size_operations = 100;
    const int numThreads = 10;
    std::atomic<int> pushCount{0};
    std::atomic<int> takeCount{0};
    auto pushTask = [&]() {
        for (int i = 0; i < size_operations; ++i) {
            auto message = generated_words(size_words);
            auto name = generated_words(size_words);
             auto task = std::make_unique<TestTask>(message, name);
            if((queue_.push(std::move(task))))
                pushCount++;       
        }
    };
    auto takeTask = [&]() {
        for (int i = 0; i < size_operations; ++i) {
            std::unique_ptr<TestTask> task_ptr;
            while (!(task_ptr = queue_.take()));
            takeCount++;     
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
}

TEST_F(ThreadSafeQueueTest, TestPushAndTakeConcurrent) {
    Queue<TestTask> queue_;
    std::atomic<int> tasks_taken(0);
    std::atomic<int> tasks_pushed(0);
    const size_t size_words = 10;
    const size_t size_operations = 20;

    auto push_task = [&]() {
        for (size_t i = 0; i < size_operations; ++i) {
            auto message = generated_words(size_words);
            auto name = generated_words(size_words);
            auto task = std::make_unique<TestTask>(message, name);
            if (queue_.push(std::move(task))) {
                tasks_pushed++;
            }
        }
    };
    auto take_task = [&]() {
        for (size_t i = 0; i < size_operations; ++i) {
            std::unique_ptr<TestTask> task;
            while(!(task = queue_.take()));
            tasks_taken++;
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
}

TEST_F(ThreadSafeQueueTest, TestTakeFromEmptyQueue) {

    Queue<TestTask> queue_;
    std::atomic<int> tasks_taken(0);
    const size_t size_operations = 10000;
    auto take_task = [&]() {
        for (size_t i = 0; i < size_operations; ++i) {
            auto task = queue_.take();
            if (task) {
                tasks_taken++;
            }
        }
    };
    std::thread t1(take_task);
    std::thread t2(take_task);
    t1.join();
    t2.join();

    EXPECT_EQ(tasks_taken.load(), 0);
}

TEST_F(ThreadSafeQueueTest, SingleThreadedPushTakeOfSetTest) {
    Queue<TestTask> queue_(100);
    std::mutex set_mutex;
    const int size_words = 5;
    const int size_operations = 50;
    std::unordered_set<TestTask> t_set;

    auto pushTask = [&]() {
        for (int i = 0; i < size_operations; ++i) {
            auto message = generated_words(size_words);
            auto name = generated_words(size_words);
            auto task = std::make_unique<TestTask>(message, name);
            std::lock_guard<std::mutex> lock(set_mutex);
            if(queue_.push(std::move(task))){
                t_set.insert({message, name});
            }
        }
    };
    auto takeTask = [&]() {
        for (int i = 0; i < size_operations; ++i) {
            std::unique_ptr<TestTask> task_ptr;
            while (!(task_ptr = queue_.take()));
            std::lock_guard<std::mutex> lock(set_mutex);
            auto it = t_set.find(*task_ptr);
            if (it != t_set.end()){
                t_set.erase(it);
            }
        }
    };
    std::thread pushThreads{pushTask};
    std::thread takeThreads{takeTask};

    pushThreads.join();
    takeThreads.join();
    
    ASSERT_EQ(t_set.size(), 0);
}