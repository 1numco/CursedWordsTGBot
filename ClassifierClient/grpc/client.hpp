#pragma once

#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <cctype> // для std::isdigit
#include <map>
#include <chrono>
#include <grpcpp/grpcpp.h>
#include "toxicity_classifier.grpc.pb.h"
#include "toxicity_classifier.pb.h"

#include "../client.hpp"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using toxicity_classifier::MessageRequest;
using toxicity_classifier::MessageResponse;
using toxicity_classifier::ToxicityClassifier;

class ToxicityClassifierClient final: public IClassifierClient {
public:
    ToxicityClassifierClient(std::shared_ptr<Channel> channel);
    float ClassifyMessage(const std::string& message) override;
private:
    std::unique_ptr<ToxicityClassifier::Stub> stub_;
};


class ToxicityClassifierClientFactory final: public IClassifierFactory{
public:
    ToxicityClassifierClientFactory();
    std::unique_ptr<IClassifierClient> Create() override;
};
