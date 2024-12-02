#include "onethreaded_tests.hpp"

void QueueTest::SetUp() {

}

void QueueTest::TearDown() {

}

TEST_F(QueueTest, NewQueueFront) {
    auto task = queue.take();
    EXPECT_FALSE(task);
}

TEST_F(QueueTest, Front) {
    queue.push(std::make_unique<TestTask>("Hello", "Vlad"));
    auto task_ptr = queue.take();
    EXPECT_EQ(task_ptr->message_, "Hello");
    EXPECT_EQ(task_ptr->name_, "Vlad");
}

TEST_F(QueueTest, EqualFrontElements) {
    const size_t size = 100;
    std::deque<std::unique_ptr<TestTask>> queue_temp;
    for(size_t i = 0; i < size; i++){
      queue.push(std::make_unique<TestTask>("Hello", "Vasya"));
      queue_temp.push_back(std::make_unique<TestTask>("Hello", "Vasya"));
      auto task_ptr = queue.take();;
      EXPECT_EQ(task_ptr->message_, queue_temp[0]->message_);
  }
}

TEST_F(QueueTest, EqualElements) {
    const size_t size_queue = 100;
    const size_t size_words = 10;
    std::deque<std::unique_ptr<TestTask>> queue_temp;
    for(size_t i = 0; i < size_queue; i++){
      auto message = generated_words(size_words);
      auto name = generated_words(size_words);
      queue.push(std::make_unique<TestTask>(message,name)) ;
      queue_temp.push_back(std::make_unique<TestTask>(message,name));
      std::unique_ptr<TestTask> task = queue.take();
      EXPECT_EQ(task->message_, queue_temp[i]->message_);
      EXPECT_EQ(task->name_, queue_temp[i]->name_);
  }
}

TEST_F(QueueTest, EqualFrontElementsAfterRemove) {
    const size_t size_queue = 100;
    const size_t size_words = 10;
    std::deque<std::unique_ptr<TestTask>> queue_temp;
    for(size_t i = 0; i < size_queue; i++){
      auto message = generated_words(size_words);
      auto name = generated_words(size_words);
      queue.push(std::make_unique<TestTask>(message, name)) ;
      queue_temp.push_back(std::make_unique<TestTask>(message, name));
    }
    for(size_t i = 0; i < size_queue; i++){
      std::unique_ptr<TestTask> task = queue.take();
      std::unique_ptr<TestTask> task_temp = std::move(queue_temp.front());
      EXPECT_EQ(task->message_, task_temp->message_);
      EXPECT_EQ(task->name_, task_temp->name_);
      queue_temp.pop_front();
    }
}