
#include <vector>
#include <string>
#include <stdio.h>
#include <tgbot/tgbot.h>
//#include <sqlitecpp/sqlitecpp.h>
#include <iostream>

//using namespace std;

struct Task{
    std::string message;
    std::string chat_title;
    std::string firstName;
    std::string lastName;
    std::int64_t id_chat;
    std::int64_t id_m;
    Task(std::string mss, std::string cht, std::string fN, std::string lN, std::int64_t id_ch, std::int64_t id): message(mss), chat_title(cht),firstName(fN), lastName(lN), id_chat(id_ch), id_m(id){

     }
};

int main() {

    std::vector<Task> v_mes;

    TgBot::Bot bot("7229787403:AAH0DVCx0wUQ-G9lkXYoIllHL0DhmdawEZo");
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });
    bot.getEvents().onAnyMessage([&bot, &v_mes](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());

        v_mes.emplace_back(Task(message->text, message->chat->title, message->from->firstName, message->from->lastName,message->chat->id, message->from->id));
        
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        for(auto one: v_mes)
                std::cout<<"Message is "<<one.message<<", chat title is  "<<one.chat_title
                <<", first name is "<<one.firstName<<", last name is "<<one.lastName<<", id chat is "
                <<one.id_chat <<", id of message is "<<one.id_m<< '\n';

        bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
    });
    
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        // for(auto one: v_mes)
        //     std::cout<<one->text<<'\n';

        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}