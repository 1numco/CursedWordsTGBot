#include "worker.hpp"

Worker::Worker(std::shared_ptr<Queue<ITask>> queue_ptr):queue_ptr_(queue_ptr){

}

void Worker::terminate(){
    shutdown_requested = true;
}

void Worker::run(){
    std::unique_ptr<ITask> task_ptr;
    while ((task_ptr = queue_ptr_->take()) || !shutdown_requested ){
        if (task_ptr) {
            try {
                task_ptr->execute();
            } catch (const std::exception& e) {
                Logger::getInstance().logInfo(Logger::Levels::Critical, "Error executing task: " + std::string(e.what()));
            } 
        }
    }
}

Worker::~Worker(){

}