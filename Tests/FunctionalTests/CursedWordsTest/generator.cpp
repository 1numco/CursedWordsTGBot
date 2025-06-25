#include "generator.hpp"


Generator::Generator(std::string token): token_(token){

        t_bot = std::make_shared<TgBot::Bot>(token_);
        count_recieve_messages = 0;
        chat_id_ = -1002432345513;
}

void Generator::generator() {
    
    std::ifstream inputFile(filePath_);
    if (!inputFile) {
        Logger::getInstance().logInfo(Logger::Levels::Critical, "Не удалось открыть файл!\n");
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        size_t last_space = line.find_last_of(' ');
        if (last_space == std::string::npos) continue;

        std::string flag_str = line.substr(last_space + 1);
        bool flag = (flag_str == "1");
        std::string clean_line = line.substr(0, last_space);

        message_container[clean_line] = flag;
        
        t_bot->getApi().sendMessage(chat_id_, clean_line);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}