#include "cursedWordsTest.hpp"

void ReactorResultTest::TearDown() {

}

void ReactorResultTest::SetUp() {
    std::string token = "7212434431:AAFLuR1mQTqpageO7x575hkQzW7DzJTXdNs";
    t_bot = std::make_shared<TgBot::Bot>(token);
    count_recieve_messages = 0;
    chat_id_ = -1002432345513;
}

void ReactorResultTest::generator(){
    
    const char* filePath = std::getenv("MESSAGES_FILE_PATH");
    if (!filePath) {
        filePath = "./Tests/FunctionalTests/CursedWordsTest/messages.txt";  // По умолчанию для локальной машины
    }
    std::ifstream inputFile(filePath);
    
    if (!inputFile) {
        Logger::getInstance().logInfo(Logger::Levels::Critical, "Не удалось открыть файл!\n");
    }
    std::string line;
    while (std::getline(inputFile, line)) {
        size_t last_space = line.find_last_of(' ');
        std::string flag_str = line.substr(last_space + 1);
        bool flag = (flag_str == "1");
        std::string clean_line = line.substr(0, last_space);
        message_container[clean_line] = flag;
        t_bot->getApi().sendMessage(chat_id_, clean_line);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    inputFile.close();
}

void ReactorResultTest::checker() {
    auto last_change_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = std::chrono::duration<double>::zero();

    t_bot->getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        bool react_m = (message->text == "мат!");
        
        ASSERT_EQ(message_container[message->replyToMessage->text], react_m);

        count_recieve_messages++;
        Logger::getInstance().logInfo(Logger::Levels::Info, "count_recieve_messages: " + std::to_string(count_recieve_messages) + "\n");

        last_change_time = std::chrono::steady_clock::now();
    });

    try {
        TgBot::TgLongPoll longPoll(*t_bot);

        while (count_recieve_messages < limit_sent_messages_ && elapsed_seconds.count() < limit_time_in_sec) {

            longPoll.start();

            elapsed_seconds = std::chrono::steady_clock::now() - last_change_time;
            Logger::getInstance().logInfo(Logger::Levels::Info, "elapsed: " + std::to_string(elapsed_seconds.count()) + "\n");
        }

    } catch (TgBot::TgException& e) {
        Logger::getInstance().logInfo(Logger::Levels::Critical, std::string("Error") + e.what());
    }
}

TEST_F(ReactorResultTest, FirstTest) {

    std::jthread mainThread{[&](){
        run_bot("7763682966:AAEFBGifblSqB5of8cyS5WKjC6kK6pxTIuY");
    }};    

    generator();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::raise(SIGINT);

    checker();
}