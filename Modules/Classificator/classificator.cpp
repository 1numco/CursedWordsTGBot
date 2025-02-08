#include "classificator.hpp"

SimpleClassificator::SimpleClassificator(const std::string& message): message_(message){

}

bool SimpleClassificator::check(){
    return true;
}

SimpleClassificator::~SimpleClassificator(){

}