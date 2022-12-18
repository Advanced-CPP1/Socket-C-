#include <iostream>
#include <Winsock2.h>
#include <Ws2tcpip.h>

int main()
{
    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Create a socket
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Bind the socket to a local address and port
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(12345);
    result = bind(listenSocket, (sockaddr*)&addr, sizeof(addr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    result = listen(listenSocket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        std::cerr << "Failed to listen on socket: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Listening for incoming connections on port 12345..." << std::endl;

    // Accept an incoming connection
    SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to accept incoming connection: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Accepted incoming connection" << std::endl;

    // Send a message to the client
    const char* message = "Hello, client!\n";
    result = send(clientSocket, message, strlen(message), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Failed to send message to client: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Sent message to client: " << message << std::endl;
    
    return 0;
}
