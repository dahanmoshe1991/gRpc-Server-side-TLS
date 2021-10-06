// greeterTlsClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "CommChannel.h"
#include <iostream>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

CommChannel* m_Channel;

bool sayHello(std::string name) {
	// Each request must have its own context
	ClientContext context;
	// This will hold our request message
	helloworld::HelloRequest request;

	request.set_name(name);
	// This wil hold the response message 
	helloworld::HelloReply reply;
	std::cout << "Sending Hello to Server" << std::endl;
	// Call the function
	Status status = (m_Channel->GetStub())->SayHello(&context, request, &reply);
	// We return the status to check the connecteion
	if (status.error_code() != grpc::OK) {
		std::cout << "gRPC Error - Code: " << status.error_code() << ", Message: " << status.error_message();
		return false;
	}
	std::cout << "Received: " << reply.message() << std::endl;
	return true;
}

int main()
{
	std::cout << "Started Run" << std::endl;
	char input = 'a';
	m_Channel = new CommChannel();
	try {
		bool isConnected = m_Channel->CreateChannel("localhost", "8085");
		if (isConnected) {
			std::cout << "Enter any key to say hello th Server, enter q for exit" << std::endl;
			while (input != 'q') {
				sayHello("cpp_client");
				std::cin >> input;
			}
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << "rpc failed, error: " << ex.what() << std::endl;
	}
	std::cout << "Enter any key to close" << std::endl;
	std::cin >> input;
}