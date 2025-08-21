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
TEST_F(ThreadSafeQueueTest, PushBlocksWhenQueueIsFullAndUnblocksAfterTake) {
    const int size_of_queue = 10;
    Queue<TestTask> queue_(size_of_queue);

    for (int i = 0; i < size_of_queue; ++i) {
        auto task = std::make_unique<TestTask>("message_" + std::to_string(i), "name_" + std::to_string(i));
        ASSERT_TRUE(queue_.push(std::move(task)));
    }

    std::atomic<bool> push_finished{false};
    std::thread pushing_thread([&]() {
        auto extra_task = std::make_unique<TestTask>("extra", "task");
        queue_.push(std::move(extra_task)); 
        push_finished = true;
    });


    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(push_finished.load()) << "Push не должен был завершиться — очередь полная";

    auto taken = queue_.take();
    ASSERT_TRUE(taken != nullptr);
    EXPECT_TRUE(taken->message_.rfind("message_", 0) == 0);

    pushing_thread.join();
    EXPECT_TRUE(push_finished.load()) << "Push должен завершиться после освобождения места";
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