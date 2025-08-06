#include "client.hpp"


ToxicityClassifierClient::ToxicityClassifierClient(std::shared_ptr<Channel> channel) : stub_(ToxicityClassifier::NewStub(std::move(channel))) {
  
}

double ToxicityClassifierClient::ClassifyMessage(const std::string& message) {

    MessageRequest request;
    request.set_message(message);
    MessageResponse response;
    ClientContext context;

    Status status = stub_->ClassifyMessage(&context, request, &response);

    if (status.ok()) {
        return response.toxicity_probability();
    } else {
        throw std::runtime_error("RPC failed: " + status.error_message());
    }
}

ToxicityClassifierClientFactory::ToxicityClassifierClientFactory() {

    static const char* server_address = std::getenv("GRPC_SERVER_ADDRESS");
    if (!server_address) {
        throw std::runtime_error("GRPC_SERVER_ADDRESS environment variable not set");
    }
    channel_ = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
}

std::unique_ptr<IClassifierClient> ToxicityClassifierClientFactory::Create() {
    return std::make_unique<ToxicityClassifierClient>(channel_);
}