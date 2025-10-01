#pragma once

#include <deque>
#include <mutex>
#include <optional>
#include <memory>
#include <condition_variable>

template <class Type>
class Queue {

public:
    Queue(const size_t limit = 100);
    void push( std::unique_ptr<Type> task);
    std::unique_ptr<Type> take();
    void shutdown();
    ~Queue();

private:
    const size_t limit_;
    std::deque<std::unique_ptr<Type>> deque_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::condition_variable cv_full_;
    std::atomic<bool> shutdown_ = false; 
};

template <class Type>
Queue<Type>::Queue(const size_t limit): limit_(limit){

}

template <class Type>
void Queue<Type>::shutdown() {
    shutdown_.store(true);
    cv_.notify_all(); 
    cv_full_.notify_all();
}

template <class Type>
void Queue<Type>::push(std::unique_ptr<Type> task) {
    if (shutdown_)[[unlikely]]  {
        throw std::runtime_error("Queue is shutdown");
    }
    std::unique_lock lock(mutex_);
    cv_full_.wait(lock, [this]() { return deque_.size() < limit_ || shutdown_; });
    deque_.push_back(std::move(task));
    lock.unlock();
    cv_.notify_one();
}

template <class Type>
std::unique_ptr<Type> Queue<Type>::take() {

    std::unique_lock lock(mutex_);
     cv_.wait(lock, [this]() { return !deque_.empty() || shutdown_; });
    
    if (shutdown_ && deque_.empty()) {
        return nullptr; 
    }   
    auto item = std::move(deque_.front());
    deque_.pop_front();
    lock.unlock();
    cv_full_.notify_one();

    return item;
}

template <class Type>
Queue<Type>::~Queue() {}