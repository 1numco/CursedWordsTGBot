#include "classificator.hpp"

SimpleClassificator::SimpleClassificator(const std::string& message): message_(message){

}

double SimpleClassificator::check(){
    return 1.0;
}

CursedWordsClassificator::CursedWordsClassificator(std::unique_ptr<IClassifierClient> client, const std::string& message):
 client_(std::move(client)), message_(message){

}

double CursedWordsClassificator::check() {
    return client_->ClassifyMessage(message_);
}