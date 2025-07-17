#include "classificator.hpp"

SimpleClassificator::SimpleClassificator(const std::string& message): message_(message){

}

bool SimpleClassificator::check(){
    return true;
}

CursedWordsClassificator::CursedWordsClassificator(std::unique_ptr<IClassifierClient> ptr_client, const std::string& message):
 ptr_client_(std::move(ptr_client)), message_(message){

}

bool CursedWordsClassificator::check() {
    double probability = ptr_client_->ClassifyMessage(message_);
    
    if(probability > cursedwords){
        std::cout<<"TRUE probability: "<< probability<<", cursedwords: "<< cursedwords<<"\n";
        return true; 
    } 
    else{
        std::cout<<" FALSE probability: "<< probability<<", cursedwords: "<< cursedwords<<"\n";
        return false;
    }
}