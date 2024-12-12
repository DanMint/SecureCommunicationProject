#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = 3490;
    inet_pton(AF_INET, "192.168.1.177" , &serverAddress.sin_addr);

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;
    // sending data
    const char* message = "Hello, server!";
    send(clientSocket, message, strlen(message), 0);

    // closing socket
    close(clientSocket);

    return 0;
}