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

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using toxicity_classifier::MessageRequest;
using toxicity_classifier::MessageResponse;
using toxicity_classifier::ToxicityClassifier;

class ToxicityClassifierClient {
public:
    ToxicityClassifierClient(std::shared_ptr<Channel> channel);

    float ClassifyMessage(const std::string& message);

private:
    std::unique_ptr<ToxicityClassifier::Stub> stub_;
};