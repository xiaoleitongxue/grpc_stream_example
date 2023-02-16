#include "stream_test.grpc.pb.h"
#include "stream_test.pb.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>
#include <grpcpp/support/sync_stream.h>

#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::Status;
using streamtest::Image;
using streamtest::StreamTest;

class StreamTestImpl final : public StreamTest::Service {
 public:
  explicit StreamTestImpl(){};
  Status send_image(ServerContext* context, ServerReader<Image>* reader,
                    Image* image) override {
    Image im;
    while (reader->Read(&im)) {
      std::cout << im.pixel() << std::endl;
    }
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  StreamTestImpl service;
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}