#include "signalhandler.hpp"

SignalHandler* handler_ptr = nullptr;

SignalHandler::SignalHandler(std::vector<int> vec_of_signals, std::function<void()> handler){
    handler_ = handler;
    signals_ = vec_of_signals;

    if (!handler_ptr){
        handler_ptr = this;
        for(auto const& signal: vec_of_signals)
            std::signal(signal, invoke);
    } else throw std::runtime_error("Fail initialization of handler's pointer\n!");   
}

void SignalHandler::invoke(int signal_){
    handler_ptr->handler_();
}

SignalHandler::~SignalHandler(){
    handler_ptr = nullptr;
}