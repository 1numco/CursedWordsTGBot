#include "test.hpp"

void QueueTest::SetUp() {

}

void QueueTest::TearDown() {

}

TEST_F(QueueTest, PushBlocksWhenQueueIsFullAndUnblocksAfterTake) {
  const int size_of_queue = 10;
  Queue<TestTask> queue_(size_of_queue);

  // Заполняем очередь до предела
  for (int i = 0; i < size_of_queue; ++i) {
      auto task = std::make_unique<TestTask>("message_" + std::to_string(i), "name_" + std::to_string(i));
      ASSERT_TRUE(queue_.push(std::move(task)));
  }

  // Поток, который попытается вставить ещё один элемент — должен заблокироваться
  std::atomic<bool> push_finished{false};
  std::thread pushing_thread([&]() {
      auto extra_task = std::make_unique<TestTask>("extra", "task");
      queue_.push(std::move(extra_task));  // должен блокироваться
      push_finished = true;
  });

  // Даём потоку время попытаться вставить (он должен заблокироваться)
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_FALSE(push_finished.load()) << "Push не должен был завершиться — очередь полная";

  // Достаём один элемент, чтобы разблокировать push
  auto taken = queue_.take();
  ASSERT_TRUE(taken != nullptr);
  EXPECT_TRUE(taken->message_.rfind("message_", 0) == 0);  // Проверяем, что взят обычный, не "extra"

  // Ожидаем завершения потока
  pushing_thread.join();
  EXPECT_TRUE(push_finished.load()) << "Push должен завершиться после освобождения места";
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