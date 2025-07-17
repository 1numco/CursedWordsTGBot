#include "server.hpp"


Server::Server(std::unique_ptr<TgBot::Bot> ptr_bot, std::shared_ptr<Queue<ITask>> queue, std::unique_ptr<IClassifierFactory> ptr_factory): 
ptr_bot_(std::move(ptr_bot)), queue_(queue), ptr_factory_(std::move(ptr_factory)) {

    ptr_bot_->getEvents().onCommand("start", [&](TgBot::Message::Ptr message) {
        ptr_bot_->getApi().sendMessage(message->chat->id, "Hi!");
    });

    
    ptr_bot_->getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }

        toxicity_client_ = ptr_factory_->Create();
        
        if (!queue_->push(std::make_unique<CursedWordDetectingTask>(
            
            std::make_shared<CursedWordsClassificator>(std::move(toxicity_client_), message->text),
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
            Logger::getInstance().logInfo(Logger::Levels::Fatal, e.what());
        }
}

void Server::terminate(){
    shutdown_requested = true;
}

Server::~Server(){
    
}