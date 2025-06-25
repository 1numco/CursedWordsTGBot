#include <gtest/gtest.h>
#include <tgbot/tgbot.h>
#include <fstream>
#include <atomic>
#include <map>

class Checker : public ::testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;
    void fill_map();
    void run_checker();
    void message_handler(TgBot::Message::Ptr message);

    std::atomic<size_t> count_recieve_messages{0};
    std::shared_ptr<TgBot::Bot> t_bot_checker;
    std::int64_t chat_id_;
    const size_t limit_sent_messages_ = 5;
    const size_t limit_time_in_sec = 8;
    std::map<std::string, bool> message_container;
    std::mutex set_mutex;
    std::chrono::time_point<std::chrono::steady_clock> last_change_time;
    
    static inline const char* filePath_ = "./bins/Tests/FunctionalTests/messages.txt";
};