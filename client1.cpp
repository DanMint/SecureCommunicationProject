#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

void readFully(int socket, void* buffer, size_t length) {
    size_t totalBytesRead = 0;
    while (totalBytesRead < length) {
        ssize_t bytesRead = recv(socket, static_cast<char*>(buffer) + totalBytesRead, length - totalBytesRead, 0);
        if (bytesRead <= 0) {
            std::cerr << "Error or connection closed while reading from socket" << std::endl;
            exit(1); 
        }
        totalBytesRead += bytesRead;
    }
}

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = 3490;
    inet_pton(AF_INET, "192.168.1.177", &serverAddress.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return 1;
    }

    int type;
    readFully(clientSocket, &type, sizeof(type));
    std::cout << "Message Type: " << type << std::endl;

    int size;
    readFully(clientSocket, &size, sizeof(size));
    std::cout << "Message Size: " << size << std::endl;

    char* message = new char[size]; 
    readFully(clientSocket, message, size);
    std::cout << "Message Content: " << message << std::endl;

    delete[] message;
    close(clientSocket);

    return 0;
}
