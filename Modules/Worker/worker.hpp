#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "task.hpp"
#include "queue.hpp"

class Worker{
    public:

    Worker(std::shared_ptr<Queue<ITask>> queue_ptr);
    void run();
    void terminate();
    ~Worker();

    private:
    std::shared_ptr<Queue<ITask>> queue_ptr_;
    std::atomic<bool> shutdown_requested{false};
};