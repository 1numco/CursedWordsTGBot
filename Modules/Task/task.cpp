#include "task.hpp"

CRTask::CRTask(std::shared_ptr<IClassificator> classificator, std::shared_ptr<IReactor> reactor): classificator_(classificator), reactor_(reactor){

}

void CRTask::execute(){
    reactor_->react(classificator_->check());

}

CRTask::~CRTask(){

}


CursedWordDetectingTask::CursedWordDetectingTask(std::shared_ptr<IClassificator> classificator, std::shared_ptr<IReactor> reactor)
: CRTask(classificator, reactor){

}


CursedWordDetectingTask::~CursedWordDetectingTask(){

}