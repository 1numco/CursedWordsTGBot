#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <mutex>
#include <memory>
#include <string>
#include <chrono>
#include <ctime>
#include <source_location>

class Logger {

public:
    enum class Levels {Debug, Info, Critical, Fatal};
    
    // Получение единственного экземпляра логгера
    static Logger& getInstance() {
        static Logger instance; // Инициализация при первом вызове
        return instance;
    }

    void setLevel(const Levels& level){
        level_ = level;
    }

    void setName(const std::string& name){
        bot_name_ = name;
    }

    // Запись информационного сообщения
    void logInfo(const Levels& level, const std::string& message, const std::source_location location = std::source_location::current()) {
        if(level >= level_){

            

            const auto p1 = std::chrono::system_clock::now();
            const auto time_date_stamp =  (std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()) + std::chrono::hours(3)).count();

            std::tm* t = std::gmtime(&time_date_stamp);
            std::stringstream ss; 
            ss << std::put_time(t, "%Y-%m-%d %H:%M:%S");
            std::string output = ss.str();
        
            std::lock_guard<std::mutex> guard(logMutex);
            std::clog << "\n{Name:"<<bot_name_
            << ", level: " << levels_[level_]
            <<", time: "<< output 
            <<", file: "
            << location.file_name() << '('
            << location.line() << ':' 
            << location.column() << ")"
            <<", message: "<< message <<"}\n";
            
        }
    }

private:
    Logger() = default;

    ~Logger() {

    }

    // Запрет копирования и перемещения
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    std::mutex logMutex;   // Мьютекс для потокобезопасности
    std::string bot_name_;
    Levels level_;
    inline static std::map<Levels, std::string> levels_{
        {Levels::Debug, "Debug"},
        {Levels::Info, "Info"},
        {Levels::Critical, "Critical"},
        {Levels::Fatal, "Fatal"}
    };
};