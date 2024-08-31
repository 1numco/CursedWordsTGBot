#include "test.h"

    void QueueTest::SetUp() {
 // Создаем очередь перед каждым тестом
    }

    void QueueTest::TearDown() {

    }

TEST_F(QueueTest, NewQueueIsEmpty) {
  EXPECT_EQ(queue.size(), 0);
}


TEST_F(QueueTest, NewQueuePop) {
  //queue->pop();
  EXPECT_EQ(queue.pop(), false);
  EXPECT_EQ(queue.size(), 0);
}


TEST_F(QueueTest, Push) {
  queue.push(7);
  std::cout<<"size = "<<queue.size()<<"\n";
  EXPECT_EQ(queue.size(), 1);
}

TEST_F(QueueTest, Front) {
	queue.push(5);
  std::optional<int> task = queue.front();
  EXPECT_EQ(task.value(), 5);

}

TEST_F(QueueTest, NewQueueFront) {

  std::optional<std::reference_wrapper<int>> task = queue.front();
  EXPECT_EQ(task, std::nullopt);

}

TEST_F(QueueTest, NewQueueIsEmptyAfterAddAndPop) {
  const size_t size = 1000;
  for(size_t i = 0; i < size; i++){
    queue.push(i + 3*i);
  }
  for(size_t i = 0; i < size; i++){
    queue.pop();
  }

  EXPECT_EQ(queue.size(), 0);
}

TEST_F(QueueTest, QueueSizeAferAddAndSomePop) {
  const size_t size = 1000;
  for(size_t i = 0; i < size; i++){
    queue.push(i + 2*i);
  }
  const size_t size_ = size/4;
  for(size_t i = 0; i < size_; i++){
    queue.pop();
  }

  EXPECT_EQ(queue.size(), size - size_);
}


TEST_F(QueueTest, QueueEqualElementAferAdd) {
  const size_t size = 1000;
  std::deque<int> queue_temp;
  for(size_t i = 0; i < size; i++){
    queue.push(i + 2*i);
    queue_temp.push_back(i + 2*i);
  }

  for(size_t i = 0; i < size; i++){
    std::optional<int> value = queue.front();
    queue.pop();
    EXPECT_EQ(value.value(), queue_temp[i]);
  }
}


TEST_F(QueueTest, QueueEqualFrontElementAferAdd) {
  const size_t size = 1000;
  std::deque<int> queue_temp;
  for(size_t i = 0; i < size; i++){
    queue.push(i + 2*i);
    queue_temp.push_back(i + 2*i);
  }

  for(size_t i = 0; i < size; i++){
    std::optional<int> value = queue.front();
    EXPECT_EQ(value.value(), queue_temp[0]);
  }
}

TEST_F(QueueTest, FinishTestQueue) {

  EXPECT_EQ(queue.size(), 0);
  const size_t size = 1000;
  std::deque<int> queue_temp;
  for(size_t i = 0; i < size; i++){
    queue.push(i + 2*i);
    queue_temp.push_back(i + 2*i);
  }
  EXPECT_EQ(queue.size(), size);
  
  for(size_t i = 0; i < size; i++){
    std::optional<int> value = queue.front();
    EXPECT_EQ(value.value(), queue_temp[0]);
  }
  for(size_t i = 0; i < size; i++){
    queue.pop();
    EXPECT_EQ(queue.size(), size - i - 1);
  }

  queue_temp.clear();
  queue.push(1);
  queue.push(7);
  queue.push(11);
  EXPECT_EQ(queue.size(), 3);



}

/*

1) Добавить Н элеметов и удалить Н элементов: посмотреть размер после этих операций

2) Добавить, к примеру, M элементов и удалить N элементов и посмотреть размер- равен ли он M-N 

3) Проверить, работает ли очередь по принципу FIFO с помощью дополнительного контейнера, к примеру, вектора

4) Протестировать работу метода front(): добавить N элементов и при этом метод front() должен соответствовать всегда
одному и тому же первому элементу


*/



//std::cout<<value.value()<<", "<<queue_temp[i]<<"\n";