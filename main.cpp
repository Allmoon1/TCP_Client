#include <iostream>
#include <ostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")


void main() {

	std::string ipAddress = "82.179.143.16";
	int port = 5000;

	//Init Winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);

	if (wsOk != 0) {
		std::cerr << "Can't init winsock" << std::endl;
		return;
	}

	//Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		std::cerr << "Can't create a socket" << std::endl;
		return;
	}

	//Fill in a hint struct
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//Connect to server
	int connResult = connect(listening, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		std::cerr << "Can't connect to server" << std::endl;
		closesocket(listening);
		WSACleanup();
		return;
	}
	
	//Do while loop to send and recv data
	char buf[4096];
	std::string userInput;

	do {
		std::cout << "> ";
		std::getline(std::cin, userInput);
		if (userInput.size() > 0) {
			int sendResult = send(listening, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				ZeroMemory(buf, 4096);
				int bytesRecv = recv(listening, buf, 4096, 0);
				if (bytesRecv > 0) {
					std::cout << "SERVER> " << std::string(buf, 0, bytesRecv) << std::endl;
				}
			}
		}
		

	} while (userInput.size() > 0);
	//Close down 
	closesocket(listening);
    WSACleanup();
}