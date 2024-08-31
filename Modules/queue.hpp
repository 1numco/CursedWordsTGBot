#ifndef QUEUE_H
#define QUEUE_H

#include <deque>
#include <string>
#include <thread>
#include <mutex>
#include "task.h"
#include <optional>
#include <functional>

template <typename T>
class Queue {

private:

std::deque<T> deque;
std::mutex m1;


public:

    Queue();
    void push(T);
	size_t size();
    std::optional<std::reference_wrapper<T>> front();
    bool pop();

    T& operator[](size_t i);

    ~Queue();
};

    template <typename T>
    Queue<T>::Queue(){

    }

    template <typename T>
    void Queue<T>::push(T task){
        std::lock_guard lg(m1);
        deque.push_back(task);
    }

    template <typename T>
    std::optional<std::reference_wrapper<T>> Queue<T>::front(){
        if(!deque.empty()){
            return std::ref(deque.front());
        }
        else
            return std::nullopt;
    }

    template <typename T>
    bool Queue<T>::pop(){
        if(!deque.empty()){
            deque.pop_front();
            return true;
        }
        return false;
    }

    template <typename T>
    size_t Queue<T>::size(){
        return deque.size();
    }

    template <typename T>
    T& Queue<T>::operator[](size_t i){
        return deque[i];
    }

    template <typename T>
    Queue<T>::~Queue(){

    }

#endif // MODEL_H