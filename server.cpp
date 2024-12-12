#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <unordered_map>
#include <cassert>
#include <arpa/inet.h>

#define PORT 3490
#define BUFFER_SIZE 1024

class Test {
    public:
        static void testClientSocket(const int socket) {
            std::cout << socket << std::endl;
            assert(socket > 0 && "New client socket is incorrect");
        }
};



class Server {
    private:
        int serverSocket;
        sockaddr_in serverAddress;
        std::unordered_map<std::string, int> clinetsIpPort;
        std::unordered_map<std::string, std::string> clientsIpName;

    public:
        Server() {
            serverSocket = socket(AF_INET, SOCK_STREAM, 0);

            sockaddr_in serverAddress;
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_port = 3490;
            serverAddress.sin_addr.s_addr = INADDR_ANY;

            bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        }

        void start() {
            listen(serverSocket, 5);

            while(true) {
                sockaddr_in clientAddr;
                socklen_t addrLen = sizeof(clientAddr);
                // accept client connection
                int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
                Test::testClientSocket(clientSocket);

                char clientIP[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

                std::cout << "Client from " << clientIP << " on port " << clientSocket << std::endl;

                // while(true) {
                //     const char* message = "Hello, server!";
                //     send(clientSocket, message, strlen(message), 0);    
                // }
                // recive data
                char buffer[1024] = {0};
                recv(clientSocket, buffer, sizeof(buffer), 0);
                std::cout << "Message from client: " << buffer << std::endl;
            }
        }

        void clientInteraction(const int clientSocket) {
            // welcome message
            const char* message = "Connection established with server, please write your name to identify you by";
            size_t sizeOfMessage = sizeof(message);

            // length prefix protocol (send size of data first then send the data)
            send(clientSocket, &sizeOfMessage, sizeof(sizeOfMessage), 0);
            // seinding the welcom message
            send(clientSocket, message, strlen(message), 0);  

            // reciving response from client
            char buffer[1024];
            std::string completeString;

            while(true) {
                size_t bytesRecived = recv(clientSocket, buffer, sizeof(buffer), 0);

                if(bytesRecived == 0) {
                    std::cout << "Client disconencted as not response, disconnecting...." << std::endl;
                    return;
                } 
                else if (bytesRecived < 0) {
                    std::cout << "Error in the netwrok, disconnetcing...." << std::endl;
                    return;
                }

                completeString.append(buffer);
            }  
        }

        void closeServerSocket() {
            close(serverSocket);
        }

        void addClient(const std::string &ip, int port, const std::string &name) {
            clinetsIpPort[ip] = port;
            clientsIpName[ip] = name;
        }

        
};

int main() {
    Server newServer{};

    newServer.start();

    return 0;
}