#include "checker.hpp"

void Checker::TearDown() {

}

void Checker::SetUp() try {

    const char* token_of_checker = std::getenv("TELEGRAM_TOKEN_CHECKER");
    const char* chat_id_value = std::getenv("CHAT_ID");
    chat_id_ = std::stoll(chat_id_value);
 
    t_bot_checker = std::make_shared<TgBot::Bot>(token_of_checker);
    count_recieve_messages = 0;
    fill_map();

}catch(const std::exception& e){
    Logger::getInstance().logInfo(Logger::Levels::Critical, "SetUp failed: " + std::string(e.what()));
    throw; 
}

void Checker::fill_map() {
    std::ifstream inputFile(filePath_);
    
    if (!inputFile) {
        Logger::getInstance().logInfo(Logger::Levels::Critical, "Failed to open file!\n");
        return;
    }
    
    std::string line;
    while (std::getline(inputFile, line)) {
        size_t last_space = line.find_last_of(' ');
        if (last_space == std::string::npos) {
            Logger::getInstance().logInfo(Logger::Levels::Critical, "Invalid line format: " + line);
            continue;
        }
        
        std::string message_text = line.substr(0, last_space);
        std::string flag_str = line.substr(last_space + 1);
        bool flag = (flag_str == "1");
        
        message_container[message_text] = flag;
    }
    inputFile.close();
    Logger::getInstance().logInfo(Logger::Levels::Info, "Loaded test cases: " + message_container.size());
}

void Checker::message_handler(TgBot::Message::Ptr message) {
    if (!message->replyToMessage) {
        Logger::getInstance().logInfo(Logger::Levels::Info, "Message is not a reply: " + message->text);
        return;
    }
    
    if (message->replyToMessage->text.empty()) {
        Logger::getInstance().logInfo(Logger::Levels::Info, "Original message is empty");
        return;
    }
    
    std::string original_text = message->replyToMessage->text;
    
    auto it = message_container.find(original_text);
    if (it == message_container.end()) {
        Logger::getInstance().logInfo(Logger::Levels::Info, "Unknown message: "  + original_text);
        return;
    }
    
    bool actual_reaction = (message->text == "мат");
    bool expected_reaction = it->second;
    
    Logger::getInstance().logInfo(Logger::Levels::Info, 
    "\n| Checking message: " + original_text + 
    "\n| Expected: " + (expected_reaction == 1 ? "Мат": "Не мат") + 
    "\n| Actual: " + (actual_reaction == 1 ? "Мат": "Не мат"));

    
    ASSERT_EQ(expected_reaction, actual_reaction);
    
    count_recieve_messages++;
    last_change_time = std::chrono::steady_clock::now();
}

void Checker::run_checker() {
    last_change_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = std::chrono::duration<double>::zero();;
    
    t_bot_checker->getEvents().onAnyMessage(
        [this](TgBot::Message::Ptr message) {
            this->message_handler(message);
        }
    );
    try {
        TgBot::TgLongPoll longPoll(*t_bot_checker);
        
        while (count_recieve_messages < message_container.size() && 
               elapsed_seconds.count() < limit_time_in_sec) {
            longPoll.start();
            elapsed_seconds = std::chrono::steady_clock::now() - last_change_time;
        }
    } catch (const TgBot::TgException& e) {
        Logger::getInstance().logInfo(Logger::Levels::Info, "Telegram error: " + std::string(e.what()));
    }
    Logger::getInstance().logInfo(Logger::Levels::Info, "Check completed. Processed messages: " + std::to_string(count_recieve_messages) + " : " + std::to_string(message_container.size()));
}

TEST_F(Checker, FirstTest) {
    run_checker();
}