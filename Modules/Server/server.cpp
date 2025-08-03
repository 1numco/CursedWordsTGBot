#include "server.hpp"


Server::Server(std::unique_ptr<TgBot::Bot> ptr_bot, std::shared_ptr<Queue<ITask>> queue, std::unique_ptr<IClassifierFactory> classifier_factory): 
ptr_bot_(std::move(ptr_bot)), queue_(std::move(queue)), classifier_factory_(std::move(classifier_factory)) {

    ptr_bot_->getEvents().onCommand("start", [&](TgBot::Message::Ptr message) {
        ptr_bot_->getApi().sendMessage(message->chat->id, "Hi!");
    });

    
    ptr_bot_->getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        
        if (!queue_->push(std::make_unique<CursedWordDetectingTask>(
            
            std::make_shared<CursedWordsClassificator>(std::move(classifier_factory_->Create()), message->text),
            std::make_shared<CursedWordsReactor>(ptr_bot_, message->text, message->chat->id, message->messageId)
        
        ))){
            Logger::getInstance().logInfo(Logger::Levels::Critical, "Queue is full!"); 
            std::this_thread::sleep_for (std::chrono::milliseconds(100));
        }
    });
}

void Server::start(){
    try {                                                                                                                                           
        TgBot::TgLongPoll longPoll(*ptr_bot_);

        while (!shutdown_requested) {
            longPoll.start();
        }
        
        } catch (TgBot::TgException& e) {
            Logger::getInstance().logInfo(Logger::Levels::Fatal, "An error occurred in the bot: " + std::string(e.what()));
        }
}

void Server::terminate(){
    shutdown_requested = true;
}

Server::~Server(){
    
}