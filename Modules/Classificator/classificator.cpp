#include "classificator.hpp"

SimpleClassificator::SimpleClassificator(const std::string& message): message_(message){

}

bool SimpleClassificator::check(){
    return true;
}

SimpleClassificator::~SimpleClassificator(){

}

float CursedWordsClassificator::cursedwords = 0.5;

CursedWordsClassificator::CursedWordsClassificator(std::unique_ptr<IClassifierClient> ptr_client, const std::string& message):
 ptr_client_(std::move(ptr_client)), message_(message){

}

bool CursedWordsClassificator::check() {
    float probability = ptr_client_->ClassifyMessage(message_);
    
    if(probability > cursedwords) return true; 
    else return false;
}

CursedWordsClassificator::~CursedWordsClassificator(){ }