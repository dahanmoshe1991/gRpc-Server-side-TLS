#pragma once
#include "greet.grpc.pb.h"

class CommChannel {
public:
	// Create channel and check connection
	bool CreateChannel(const std::string& ipAdress, const std::string& port);
	// Get the stub
	std::shared_ptr<helloworld::Greeter::Stub> GetStub();
	std::string readFileIntoString(const std::string& path);

private:
	// our channel
	std::shared_ptr<grpc::Channel>  m_Channel;
	// our stub
	std::shared_ptr<helloworld::Greeter::Stub> m_Stub;
};