#include "task.hpp"

BaseMessageProcessingBotTask::BaseMessageProcessingBotTask(std::shared_ptr<IClassificator> classificator, std::shared_ptr<IReactor> reactor): classificator_(std::move(classificator)), reactor_(std::move(reactor)){

}

void BaseMessageProcessingBotTask::execute(){
    reactor_->react(classificator_->check());
}

BaseMessageProcessingBotTask::~BaseMessageProcessingBotTask(){

}

CursedWordDetectingTask::CursedWordDetectingTask(std::shared_ptr<IClassificator> classificator, std::shared_ptr<IReactor> reactor)
: BaseMessageProcessingBotTask(std::move(classificator), std::move(reactor)){

}

CursedWordDetectingTask::~CursedWordDetectingTask(){

}