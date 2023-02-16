#include "stream_test.grpc.pb.h"
#include "stream_test.pb.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using streamtest::Image;
using streamtest::StreamTest;

class StreamTestClient {
 public:
  explicit StreamTestClient(std::shared_ptr<Channel> channel)
      : stub_(StreamTest::NewStub(channel)) {}
  void send_image() {
    Image im;
    ClientContext context;
    Image* image =new Image[3];
    image[0].set_pixel(0);
    image[1].set_pixel(1);
    image[2].set_pixel(2);
    std::unique_ptr<ClientWriter<Image> > writer(
        stub_->send_image(&context, &im));
    for(int i = 0; i < 3; ++i){
        writer->Write(image[i]);
    }
    writer->WritesDone();
    Status status = writer->Finish();
  }

 private:
  std::unique_ptr<StreamTest::Stub> stub_;
};

int main(int argc, char** argv) {
  StreamTestClient stc(grpc::CreateChannel("localhost:50051",
                       grpc::InsecureChannelCredentials()));

   stc.send_image();                    
  return 0;
}