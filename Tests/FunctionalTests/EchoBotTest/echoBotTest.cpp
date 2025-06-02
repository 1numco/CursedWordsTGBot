#include "echoBotTest.hpp"

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
        std::cout<<"in filePath\n";
        filePath = "./bins/Tests/FunctionalTests/EchoBotTest/messages.txt";  // По умолчанию для локальной машины
    }
    std::ifstream inputFile(filePath);
    
    if (!inputFile) {
        std::cout<<"error open filePath\n";
        std::cerr << "Не удалось открыть файл!" << std::endl;
    }
    std::string line;
    while (std::getline(inputFile, line)) {
        {
            std::lock_guard lg{set_mutex};
            message_container.insert(line);

        }
        try {
            t_bot->getApi().sendMessage(chat_id_, line);
        } catch (const TgBot::TgException& e) {
            std::cerr << "Ошибка отправки: " << e.what() << std::endl;
            continue; // Пропустить ошибку и продолжить цикл
        }
    }
    inputFile.close();
}

void ReactorResultTest::checker(){
    auto last_change_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = std::chrono::duration<double>::zero();

    t_bot->getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
            count_recieve_messages++;
            std::lock_guard lg{set_mutex};
            ASSERT_EQ(message_container.count(message->replyToMessage->text), true);
            last_change_time = std::chrono::steady_clock::now(); 
    });
    try {
        TgBot::TgLongPoll longPoll( *t_bot);
        while (count_recieve_messages <= limit_sent_messages_ && elapsed_seconds.count() < limit_time_in_sec) {
            longPoll.start();
            elapsed_seconds = std::chrono::steady_clock::now() - last_change_time;
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
}

TEST_F(ReactorResultTest, FirstTest) {

    std::jthread mainThread{[&](){
        run_bot("7763682966:AAEFBGifblSqB5of8cyS5WKjC6kK6pxTIuY");
    }};    

    generator();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::raise(SIGINT);
    
    checker();

}