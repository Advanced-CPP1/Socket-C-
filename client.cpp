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
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Connect to the server
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(12345);
    result = connect(clientSocket, (sockaddr*)&addr, sizeof(addr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server" << std::endl;

    // Receive a message from the server
    char buffer[256];
    result = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Failed to receive message from server: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Received message from server: " << buffer << std::endl;

    // Close the socket and cleanup Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
