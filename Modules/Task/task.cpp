#include "task.hpp"

BaseMessageProcessingBotTask::BaseMessageProcessingBotTask(std::shared_ptr<IClassificator> classificator, std::shared_ptr<IReactor> reactor): classificator_(classificator), reactor_(reactor){

}

void BaseMessageProcessingBotTask::execute(){
    reactor_->react(classificator_->check());
}

BaseMessageProcessingBotTask::~BaseMessageProcessingBotTask(){

}


CursedWordDetectingTask::CursedWordDetectingTask(std::shared_ptr<IClassificator> classificator, std::shared_ptr<IReactor> reactor)
: BaseMessageProcessingBotTask(classificator, reactor){

}


CursedWordDetectingTask::~CursedWordDetectingTask(){

}