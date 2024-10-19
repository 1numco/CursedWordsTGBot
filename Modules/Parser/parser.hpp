#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>
#include <boost/algorithm/string/split.hpp>



class Parser{

public:

    Parser(const std::vector<std::string>list);
    void remove_spaces(std::string& str);
    std::vector<std::string> split_string(const std::string str);
    std::unordered_map<std::string, std::string> parse_string(std::string str);
    std::unordered_set<std::string> get_arguments();
    ~Parser();

private:
    std::unordered_set<std::string> arguments;
};