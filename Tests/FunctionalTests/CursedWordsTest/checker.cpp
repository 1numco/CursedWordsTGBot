#include "checker.hpp"

void Checker::TearDown() {
    // Очистка ресурсов при необходимости
}

void Checker::SetUp() {
    const char* token_of_checker = std::getenv("TELEGRAM_TOKEN_CHECKER");

    t_bot_checker = std::make_shared<TgBot::Bot>(token_of_checker);
    count_recieve_messages = 0;
    fill_map();
}

void Checker::fill_map() {
    std::ifstream inputFile(filePath_);
    
    if (!inputFile) {
        Logger::getInstance().logInfo(Logger::Levels::Critical, "Не удалось открыть файл!\n");
        return;
    }
    
    std::string line;
    while (std::getline(inputFile, line)) {
        size_t last_space = line.find_last_of(' ');
        if (last_space == std::string::npos) {
            std::cerr << "Некорректный формат строки: " << line << std::endl;
            continue;
        }
        
        std::string message_text = line.substr(0, last_space);
        std::string flag_str = line.substr(last_space + 1);
        bool flag = (flag_str == "1");
        
        message_container[message_text] = flag;
    }
    inputFile.close();
    
    std::cout << "Загружено тестовых случаев: " << message_container.size() << std::endl;
}

void Checker::message_handler(TgBot::Message::Ptr message) {
    if (!message->replyToMessage) {
        std::cout << "Сообщение не является ответом: " << message->text << std::endl;
        return;
    }
    
    if (message->replyToMessage->text.empty()) {
        std::cout << "Оригинальное сообщение пустое" << std::endl;
        return;
    }
    
    std::string original_text = message->replyToMessage->text;
    
    auto it = message_container.find(original_text);
    if (it == message_container.end()) {
        std::cerr << "Неизвестное сообщение: " << original_text << std::endl;
        return;
    }
    
    bool actual_reaction = (message->text == "мат");
    bool expected_reaction = it->second;
    
    std::cout << "Проверка сообщения: " << original_text
              << " | Ожидалось: " << expected_reaction
              << " | Фактически: " << actual_reaction << std::endl;
    
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
        std::cerr << "Ошибка Telegram: " << e.what() << std::endl;
    }
    
    std::cout << "Проверка завершена. Обработано сообщений: " 
              << count_recieve_messages << "/" << message_container.size() << std::endl;
}

TEST_F(Checker, FirstTest) {
    run_checker();
}