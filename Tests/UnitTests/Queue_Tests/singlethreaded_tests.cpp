#include "singlethreaded_tests.hpp"

void QueueTest::SetUp() {

}

void QueueTest::TearDown() {

}

TEST_F(QueueTest, SingleElement) {
    queue.push(std::make_unique<TestTask>("Hello", "Vlad"));
    auto task_ptr = queue.take();
    EXPECT_EQ(task_ptr->message_, "Hello");
    EXPECT_EQ(task_ptr->name_, "Vlad");
}

TEST_F(QueueTest, FIFO_Order) {
    constexpr size_t count = 100;
    std::vector<std::string> messages;
    std::vector<std::string> names;
    
    for(size_t i = 0; i < count; i++) {
        messages.push_back("Msg" + std::to_string(i));
        names.push_back("Name" + std::to_string(i));
    }
    
    for(size_t i = 0; i < count; i++) {
        queue.push(std::make_unique<TestTask>(messages[i], names[i]));
    }
    
    for(size_t i = 0; i < count; i++) {
        auto task_ptr = queue.take();
        EXPECT_EQ(task_ptr->message_, messages[i]);
        EXPECT_EQ(task_ptr->name_, names[i]);
    }
}

TEST_F(QueueTest, FIFO_WithRandomData) {
    constexpr size_t count = 100;
    constexpr size_t word_size = 10;
    
    std::vector<std::string> messages;
    std::vector<std::string> names;
    
    for(size_t i = 0; i < count; i++) {
        messages.push_back(generated_words(word_size));
        names.push_back(generated_words(word_size));
    }
    
    for(size_t i = 0; i < count; i++) {
        queue.push(std::make_unique<TestTask>(messages[i], names[i]));
    }
    
    for(size_t i = 0; i < count; i++) {
        auto task_ptr = queue.take();
        EXPECT_EQ(task_ptr->message_, messages[i]);
        EXPECT_EQ(task_ptr->name_, names[i]);
    }
}

TEST_F(QueueTest, PushBlocksWhenFull) {
    constexpr size_t max_size = 10;
    
    Queue<TestTask> small_queue(max_size);
    
    for(size_t i = 0; i < max_size; i++) {
        EXPECT_TRUE(small_queue.push(std::make_unique<TestTask>(
            generated_words(5), generated_words(5)
        )));
    }
    
    std::atomic<bool> push_done(false);
    std::thread push_thread([&] {
        small_queue.push(std::make_unique<TestTask>("Overflow", "Test"));
        push_done = true;
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_FALSE(push_done.load());
    
    auto task = small_queue.take();
    push_thread.join();
    EXPECT_TRUE(push_done.load());
}

TEST_F(QueueTest, ShutdownBehavior) {
    for(int i = 0; i < 5; i++) {
        queue.push(std::make_unique<TestTask>(
            generated_words(5), generated_words(5)
        ));
    }
    
    queue.shutdown();
    
    EXPECT_THROW(
        queue.push(std::make_unique<TestTask>("Should", "Reject")),
        std::runtime_error
    );
    
    for(int i = 0; i < 5; i++) {
        auto task = queue.take();
        EXPECT_NE(task, nullptr);
    }
    
    EXPECT_EQ(queue.take(), nullptr);
}