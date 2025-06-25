
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include "run_bot.hpp"


int main() {
    const char* token_running_bot = std::getenv("TOKEN_RUNNING_MAIN_BOT");
    
    run_bot(token_running_bot);

    return 0;
}