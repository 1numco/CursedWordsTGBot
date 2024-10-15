#pragma once

#include <deque>
#include <mutex>
#include <optional>

template <typename T>
class Queue {

public:

    Queue(const size_t limit_ = 100): limit(limit_){

    }

    bool push(T task){
        std::lock_guard lg(m1);
        if(deque.size() < limit){
            deque.push_back(task);
            return true;
        }
        return false;
    }
    
    size_t size(){
        std::lock_guard lg(m1);
        return deque.size();
    }
    
    std::optional<std::reference_wrapper<T>> front(){
        std::lock_guard lg(m1);
        if(!deque.empty()){
            return std::ref(deque.front());
        }
        return std::nullopt;
    };

    bool pop(){
        std::lock_guard lg(m1);
        if(!deque.empty()){
            deque.pop_front();
            return true;
        }
        return false;
    }

    ~Queue(){

    }

private:
    const size_t limit;
    std::deque<T> deque;
    std::mutex m1;
};


