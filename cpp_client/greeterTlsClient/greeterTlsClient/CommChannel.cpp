#include "CommChannel.h"
#include <grpcpp/grpcpp.h>
#include <grpcpp/create_channel.h>
#include "greet.grpc.pb.h"
#include <iostream>
#include <fstream>
#include <sstream>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

// TODO: Add logger
#define KEEP_ALIVE_TIME_MS 60000 // 1 min
#define MAX_MSG_SIZE 64 * 1024 * 1024 // 64 MB

// Create our channel
bool CommChannel::CreateChannel(const std::string& ipAdress, const std::string& port)
{
	// target string format is ip:port
	std::string target_str = ipAdress + ":" + port;

	// Add here Channel arguments - if needed
	grpc::ChannelArguments channelArguments;

	std::string KeysFolder = "C:\\git\\TlsTest\\Keys\\";
	std::string cacert = readFileIntoString(KeysFolder + "RootCA.crt");
	
	grpc::SslCredentialsOptions ssl_opts;
	ssl_opts.pem_root_certs = cacert;
	
	//(Future imp)-For Mutual TLS:
	//std::string clientcert = readFileIntoString(KeysFolder + "client.crt");
	//std::string clientkey = readFileIntoString(KeysFolder + "client.key");
	//ssl_opts.pem_cert_chain = clientcert;
	//ssl_opts.pem_private_key = clientkey;

	auto ssl_creds = grpc::SslCredentials(ssl_opts);
	m_Channel = grpc::CreateCustomChannel(target_str, ssl_creds, channelArguments);
	// we then generate our stub
	m_Stub = helloworld::Greeter::NewStub(m_Channel);
	// We try to get the channel state
	grpc_connectivity_state curState = m_Channel->GetState(true);

	// If we are ready after initial connection, everything connected succesfuly.
	bool isIdle  = m_Channel->WaitForStateChange(GRPC_CHANNEL_IDLE, std::chrono::system_clock::now() + std::chrono::seconds(2));
	if (!isIdle)
		std::cout << "Faild to create Channel" << std::endl;
	return isIdle;
}

std::string CommChannel::readFileIntoString(const std::string& path) {
	std::ostringstream ss;
	std::ifstream input_file(path);
	if (!input_file.is_open()) {
		std::cout << "Could not open the file - '"
			<< path << "'" << std::endl;
		exit(EXIT_FAILURE);
		return "";
	}
	ss << input_file.rdbuf();
	return ss.str();
}

// Get the stub
std::shared_ptr<helloworld::Greeter::Stub> CommChannel::GetStub() {
	return m_Stub;
}