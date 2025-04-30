#include "client.hpp"



  ToxicityClassifierClient::ToxicityClassifierClient(std::shared_ptr<Channel> channel) : stub_(ToxicityClassifier::NewStub(channel)) {
    
  }

  float ToxicityClassifierClient::ClassifyMessage(const std::string& message) {

      std::cout << "Sending message: " << message << std::endl;

      MessageRequest request;
      request.set_message(message);

      MessageResponse response;
      ClientContext context;

      Status status = stub_->ClassifyMessage(&context, request, &response);

      if (status.ok()) {
        std::cout << "Received response: " << response.toxicity_probability() << std::endl;
        return response.toxicity_probability();
      } else {
        std::cout << "RPC failed: " << status.error_message() << std::endl;
        return -1;
      }
  }