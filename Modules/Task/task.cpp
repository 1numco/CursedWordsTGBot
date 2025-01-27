#include "task.hpp"

CWTask::CWTask(std::shared_ptr<IClassificator> classificator, std::shared_ptr<IReactor> reactor): classificator_(std::move(classificator)), reactor_(std::move(reactor)){

}

void CWTask::execute(){
    reactor_->react(classificator_->check());

}

CWTask::~CWTask(){

}


CursedWordDetectingTask::CursedWordDetectingTask(std::shared_ptr<IClassificator> classificator, std::shared_ptr<IReactor> reactor)
: CWTask(std::move(classificator), std::move(reactor)){

}


CursedWordDetectingTask::~CursedWordDetectingTask(){

}