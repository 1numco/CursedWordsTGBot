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
                if(!pushCount%10) std::cout<<"pushTask: "<<pushCount<<"\n";
            }
            bool pushed = queue_.push(std::move(task));
            if (!pushed) {
                break; // Очередь закрыта, прекращаем добавление
            }
        }
        queue_.shutdown(); // Сигнал потребителю
    };
    
    auto takeTask = [&]() {
        while (true) {
            auto task_ptr = queue_.take();
            if (!task_ptr) break; // Выход при shutdown
            
            //std::unique_lock lock(set_mutex);
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
            
            // Пытаемся добавить задачу. Если очередь закрыта, выходим.
            if (!queue_.push(std::move(task))) {
                break;
            }
        }
    };

    // Запускаем три потока-производителя
    std::jthread pushThreads_one(pushTask);
    std::jthread pushThreads_two(pushTask);
    std::jthread pushThreads_three(pushTask);
    
    // Потоки автоматически останавливаются при разрушении jthread


    // Закрываем очередь, чтобы take() не блокировался
    queue_.shutdown();

    // Подсчитываем оставшиеся задачи в очереди
    int count = 0;
    while (auto task = queue_.take()) {
        ++count;
    }

    // Проверяем, что количество не превысило лимит
    ASSERT_LE(count, size_of_queue) << "Queue exceeded the limit!";
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
            if (!task_ptr) continue; // Пропускаем nullptr
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
        }

        //queue_.shutdown();

        for (int i = 0; i < numThreads; ++i) {
            takeThreads[i] = std::jthread(takeTask);
        }    
    }


    ASSERT_EQ(pushCount, takeCount);
    ASSERT_EQ(t_set.size(), 0);
}