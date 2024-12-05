#include "worker.hpp"

Worker::Worker(std::shared_ptr<Queue<ITask>> queue_ptr):queue_ptr_(queue_ptr){

}

void Worker::terminate(){
    shutdown_requested = true;
}

void Worker::run(){
   std::unique_ptr<ITask> task_ptr;
    while ((task_ptr = queue_ptr_->take()) || !shutdown_requested ){
        if (task_ptr)
            task_ptr->execute();
    }
}

Worker::~Worker(){

}