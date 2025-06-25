#include "generator.hpp"
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

int main() {
    
    const char* token_of_generator = std::getenv("TELEGRAM_TOKEN_GENERATOR");
    Generator generator(token_of_generator);
    generator.generator();

    return 0;
}