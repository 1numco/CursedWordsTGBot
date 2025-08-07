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
    bool push( std::unique_ptr<Type> task);
    std::unique_ptr<Type> take();
    void shutdown();
    ~Queue();

private:
    const size_t limit_;
    std::deque<std::unique_ptr<Type>> deque;
    std::mutex mutex;
    std::condition_variable cv_;
    std::condition_variable cv_full;
    bool shutdown_ = false; 
};

template <class Type>
Queue<Type>::Queue(const size_t limit): limit_(limit){

}

template <class Type>
void Queue<Type>::shutdown() {
    {
        std::lock_guard lock(mutex);
        shutdown_ = true;
    }
    cv_.notify_all(); 
    cv_full.notify_all();
}


template <class Type>
bool Queue<Type>::push(std::unique_ptr<Type> task) {
    std::unique_lock lock(mutex);
    cv_full.wait(lock, [this]() { return deque.size() < limit_ || shutdown_; });
    if (shutdown_) {
        return false;
    }
    deque.push_back(std::move(task));
    cv_.notify_one();
    return true;
}

template <class Type>
std::unique_ptr<Type> Queue<Type>::take() {

    std::unique_lock lock(mutex);
     cv_.wait(lock, [this]() { return !deque.empty() || shutdown_; });
    
    if (shutdown_ && deque.empty()) {
        return nullptr; 
    }   
    auto item = std::move(deque.front());
    deque.pop_front();
    cv_full.notify_one();

    return item;
}

template <class Type>
Queue<Type>::~Queue() {}