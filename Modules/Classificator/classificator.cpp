#include "classificator.hpp"

SimpleClassificator::SimpleClassificator(const std::string& message): message_(message){

}

double SimpleClassificator::check(){
    return 1.0;
}

CursedWordsClassificator::CursedWordsClassificator(std::unique_ptr<IClassifierClient> ptr_client, const std::string& message):
 ptr_client_(std::move(ptr_client)), message_(message){

}

double CursedWordsClassificator::check() {
    return ptr_client_->ClassifyMessage(message_);
}