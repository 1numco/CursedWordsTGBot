#include "parser.hpp"


    Parser::Parser(const std::vector<std::string>list){
        std::copy(list.begin(), list.end(), std::inserter(arguments, arguments.end()));
    }

    void Parser::remove_spaces(std::string& str){
        std::string::iterator new_end = std::unique(str.begin(), str.end(), [](char lhs, char rhs){
            return (lhs == rhs) && (lhs == ' ');
            });
        str.erase(new_end, str.end());
    }

    std::vector<std::string> Parser::split_string(const std::string str){
        std::vector<std::string> substrings;
        boost::split(substrings, str, isspace);
        return substrings;
    }

    std::unordered_map<std::string, std::string> Parser::parse_string(std::string str){

        this->remove_spaces(str);
        auto substrings = this->split_string(str);
        std::unordered_map<std::string, std::string> tokens;

        for(size_t i = 0; i < substrings.size();){

            if(arguments.count(substrings[i])) {
                
                if(i + 1 < substrings.size() && !arguments.count(substrings[i + 1])){
                    tokens[substrings[i]] = substrings[i + 1];
                    i = i + 2;
                }
                else{
                    throw std::invalid_argument("Empty token:" + substrings[i]);
                }
            }
            else{
                throw std::invalid_argument("Unknow tokens: " + substrings[i]);
            }
         }

        if(tokens.size() > arguments.size()){
            std::string unknow_tokens;

            for(const auto& [key, value] : tokens){
                if(!arguments.count(key)){
                    unknow_tokens = unknow_tokens + key + ' ';
                }             
            }
            throw std::invalid_argument("Unknow tokens: " + unknow_tokens);

        }

        return tokens;
    }

    Parser::~Parser(){
        
    }