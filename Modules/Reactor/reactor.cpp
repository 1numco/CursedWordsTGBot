#include "reactor.hpp"

EchoReactor::EchoReactor(std::shared_ptr<TgBot::Bot> ptr_bot, std::string message, std::int64_t chat_id, std::int32_t messageId): 
ptr_bot_(std::move(ptr_bot)), message_(std::move(message)), chat_id_(chat_id), messageId_(messageId){
    
}

void EchoReactor::react(bool classification_result){

    ptr_bot_->getApi().sendMessage(chat_id_, message_, nullptr, std::make_shared<TgBot::ReplyParameters>(messageId_, chat_id_));
    Logger::getInstance().logInfo(Logger::Levels::Info, message_);
}

EchoReactor::~EchoReactor(){

}

CursedWordsReactor::CursedWordsReactor(std::shared_ptr<TgBot::Bot> ptr_bot, std::string message, std::int64_t chat_id, std::int32_t messageId): 
ptr_bot_(std::move(ptr_bot)), message_(std::move(message)), chat_id_(chat_id), messageId_(messageId){

}

void CursedWordsReactor::react(bool classification_result){
    ptr_bot_->getApi().sendMessage(chat_id_, (classification_result == true ? "мат" : "не мат"), nullptr, std::make_shared<TgBot::ReplyParameters>(messageId_, chat_id_));
    Logger::getInstance().logInfo(Logger::Levels::Info, message_ + " " + (classification_result == true ? "мат" : "не мат"));
}

CursedWordsReactor::~CursedWordsReactor(){

}