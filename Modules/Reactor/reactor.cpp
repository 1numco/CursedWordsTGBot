#include "reactor.hpp"

EchoReactor::EchoReactor(std::shared_ptr<TgBot::Bot> ptr_bot, std::string message, std::int64_t chat_id, std::int32_t messageId, TgBot::User::Ptr from): 
ptr_bot_(ptr_bot), message_(message), chat_id_(chat_id), messageId_(messageId), from_(from){

}

void EchoReactor::react(bool classification_result){

    ptr_bot_->getApi().sendMessage(chat_id_, "Message with ID " + std::to_string(messageId_) + " is cursed word!",
     nullptr, std::make_shared<TgBot::ReplyParameters>(messageId_, chat_id_)); //
    Logger::getInstance().logInfo(Logger::Levels::Info, ("Message with ID " + std::to_string(messageId_) + " is cursed word!"));
}

EchoReactor::~EchoReactor(){

}