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
#include "run_bot.hpp"
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
        
        run_bot(tokens["-token"]);
 
    }catch(std::exception const& ex){
        Logger::getInstance().logInfo(Logger::Levels::Info, ex.what());
        std::exit(EXIT_FAILURE);
    }

    return 0;
};