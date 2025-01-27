#include "echoBotTest.hpp"

void ReactorResultTest::TearDown() {

}

void ReactorResultTest::SetUp() {
    std::string token = "7389966079:AAHXCquKT0JaQUqHRzac8MMsXMCUUd5uvXQ";
    t_bot = std::make_shared<TgBot::Bot>(token);
    count_recieve_messages = 0;
    chat_id_ = -1002432345513;
}

void ReactorResultTest::generator(){
    std::ifstream inputFile("./messages.txt");
    if (!inputFile) {
        std::cerr << "Не удалось открыть файл!" << std::endl;
    }
    std::string line;
    while (std::getline(inputFile, line)) {
        {
            std::lock_guard lg{set_mutex};
            message_container.insert(line);
        }
        t_bot->getApi().sendMessage(chat_id_, line);
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
        while (count_recieve_messages <= limit_sent_messages_ && elapsed_seconds.count() < limit_time_) {
            longPoll.start();
            elapsed_seconds = std::chrono::steady_clock::now() - last_change_time;
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
}

TEST_F(ReactorResultTest, FirstTest) {

    std::jthread mainThread{[&](){
        run_bot("7229787403:AAH0DVCx0wUQ-G9lkXYoIllHL0DhmdawEZo");
    }};  

    generator();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    checker();

    std::raise(SIGINT);
}