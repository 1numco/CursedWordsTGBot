#include <stdio.h>
#include <csignal>
#include <tgbot/tgbot.h>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <exception>
#include <boost/algorithm/string.hpp>

#include "logger.hpp"
#include "task.hpp"
#include "queue.hpp"
#include "parser.hpp"
#include "server.hpp"
#include "worker.hpp"
#include "signalhandler.hpp"

std::string get_arguments_string(size_t argc, char**argv){
    std::vector<std::string> arguments_command_line;
    for(size_t i = 1; i < argc; i++)
        arguments_command_line.push_back(argv[i]);
    std::string joined_string = boost::algorithm::join(arguments_command_line, " ");
    return joined_string;
}

int main(int argc, char *argv[]) {
    
    try{
        std::string token = get_arguments_string(argc, argv);
        Parser parser{{"-token"}};
        std::unordered_map<std::string, std::string> tokens;
        tokens = parser.parse_string(token);
        std::unique_ptr<TgBot::Bot> ptr_bot = std::make_unique<TgBot::Bot>(tokens["-token"]);

        Logger::getInstance().setName(ptr_bot->getApi().getMe()->username);
        Logger::getInstance().setLevel(Logger::Levels::Debug);

        std::shared_ptr<Queue<ITask>> ptr_queue = std::make_shared<Queue<ITask>> ();
        Server server(std::move(ptr_bot), ptr_queue);
        Worker worker(ptr_queue);

        SignalHandler handler({ SIGINT, SIGTERM }, [&](){
                static int count = 0;
                if (!count++) {
                    server.terminate();
                    worker.terminate();
                    Logger::getInstance().logInfo(Logger::Levels::Critical, "Recieved shutdown signal. Stop polling!");
                } else {
                    Logger::getInstance().logInfo(Logger::Levels::Fatal, "Recieved second shutdown signal. Exiting!"); 
                    std::exit(EXIT_FAILURE);
                }
            }
        );
        std::thread worker_thread(&Worker::run, &worker);
        server.start();
        worker_thread.join();

    }catch(std::exception const& ex){
        Logger::getInstance().logInfo(Logger::Levels::Info, ex.what());
        std::exit(EXIT_FAILURE);
    }

    return 0;
};