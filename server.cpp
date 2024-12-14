#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <unordered_map>
#include <cassert>
#include <arpa/inet.h>
#include <any>
#include <variant>
#include <memory>

#define PORT 3490
#define BUFFER_SIZE 1024

class Test {
    public:
        static void testClientSocket(const size_t socket) {
            assert(socket > 0 && "New client socket is incorrect");
        }

        static void testSending(const int bytesSent, const int bytesExpectedToBeSent) {
            assert(bytesSent >= 0 && "Error happened while sending");
            assert(bytesSent == bytesExpectedToBeSent && "Bytes sent not congruent with expencted bytes send");
        }
};


class Server {
    private:
        enum typesSend {
            INT_ARR = 0,
            CHAR_ARR = 1,
            DOUBLE_ARR = 2,
            INT = 3,
            CHAR = 4,
            DOUBLE = 5
        };

        class Message {
            private:
                std::unique_ptr<int> type;
                std::unique_ptr<int> size;
                using MessageData = std::variant<
                                    std::unique_ptr<int []>,
                                    std::unique_ptr<char []>,
                                    std::unique_ptr<double []>,
                                    std::unique_ptr<int>,
                                    std::unique_ptr<char>,
                                    std::unique_ptr<double>
                                    >;

                MessageData message;

            public:
                Message() {
                    type = nullptr;
                    size = nullptr;
                }

                Message(const int type, const int size, std::unique_ptr<int []> message) {
                    this->type = std::make_unique<int>(type);
                    this->size = std::make_unique<int>(size);
                    this->message = std::move(message);
                }

                Message(const int type, const int size, std::unique_ptr<char []> message) {
                    this->type = std::make_unique<int>(type);
                    this->size = std::make_unique<int>(size);
                    this->message = std::move(message);
                }

                Message(const int type, const int size, std::unique_ptr<double []> message) {
                    this->type = std::make_unique<int>(type);
                    this->size = std::make_unique<int>(size);
                    this->message = std::move(message);
                }

                Message(const int type, const int size, std::unique_ptr<int> message) {
                    this->type = std::make_unique<int>(type);
                    this->size = std::make_unique<int>(size);
                    this->message = std::move(message);
                }

                Message(const int type, const int size, std::unique_ptr<char> message) {
                    this->type = std::make_unique<int>(type);
                    this->size = std::make_unique<int>(size);
                    this->message = std::move(message);
                }

                Message(const int type, const int size, std::unique_ptr<double> message) {
                    this->type = std::make_unique<int>(type);
                    this->size = std::make_unique<int>(size);
                    this->message = std::move(message);
                }

                int getType() {
                    return *type;
                }

                void setType(const int type) {
                    if (this->type == nullptr) 
                        this->type = std::make_unique<int>(type);
                    else 
                        *this->type = type;
                }

                int getSize() {
                    return *size;
                }

                void setSize(const int size) {
                    if (this->size == nullptr)
                        this->size = std::make_unique<int>(size);
                    else 
                        *this->size = size;
                }

                MessageData getMessage() const {
                    if (std::holds_alternative<std::unique_ptr<int[]>>(message)) {
                        const int* arr = std::get<std::unique_ptr<int[]>>(message).get();
                        auto newArray = std::make_unique<int[]>(*size);
                        for (int i = 0; i < *size; ++i) {
                            newArray[i] = arr[i];
                        }
                        return MessageData(std::move(newArray));

                    } 
                    else if (std::holds_alternative<std::unique_ptr<char[]>>(message)) {
                        const char* arr = std::get<std::unique_ptr<char[]>>(message).get();
                        auto newArray = std::make_unique<char[]>(*size);
                        for (int i = 0; i < *size; ++i) {
                            newArray[i] = arr[i];
                        }
                        return MessageData(std::move(newArray));

                    } 
                    else if (std::holds_alternative<std::unique_ptr<double[]>>(message)) {
                        const double* arr = std::get<std::unique_ptr<double[]>>(message).get();
                        auto newArray = std::make_unique<double[]>(*size);
                        for (int i = 0; i < *size; ++i) {
                            newArray[i] = arr[i];
                        }
                        return MessageData(std::move(newArray));

                    } 
                    else if (std::holds_alternative<std::unique_ptr<int>>(message)) {
                        int val = *std::get<std::unique_ptr<int>>(message).get();
                        auto newVal = std::make_unique<int>(val);
                        return MessageData(std::move(newVal));

                    } 
                    else if (std::holds_alternative<std::unique_ptr<char>>(message)) {
                        char val = *std::get<std::unique_ptr<char>>(message).get();
                        auto newVal = std::make_unique<char>(val);
                        return MessageData(std::move(newVal));

                    } 
                    else if (std::holds_alternative<std::unique_ptr<double>>(message)) {
                        double val = *std::get<std::unique_ptr<double>>(message).get();
                        auto newVal = std::make_unique<double>(val);
                        return MessageData(std::move(newVal));
                    }

                    throw std::runtime_error("Unknown type in message variant");
                }


                void setMessage(std::unique_ptr<int []> message) {
                    this->message = std::move(message);
                }

                void setMessage(std::unique_ptr<char []> message) {
                    this->message = std::move(message);
                }

                void setMessage(std::unique_ptr<double []> message) {
                    this->message = std::move(message);
                }

                void setMessage(std::unique_ptr<int> message) {
                    this->message = std::move(message);
                }

                void setMessage(std::unique_ptr<char> message) {
                    this->message = std::move(message);
                }

                void setMessage(std::unique_ptr<double> message) {
                    this->message = std::move(message);
                }

                void printMessage(const MessageData &message) {
                    if (std::holds_alternative<std::unique_ptr<char[]>>(message)) {
                        const char* arr = std::get<std::unique_ptr<char[]>>(message).get();
                        std::cout << "Message Type: char[]" << std::endl;
                        std::cout << "Message Content: " << arr << std::endl;

                    } 
                    else if (std::holds_alternative<std::unique_ptr<int[]>>(message)) {
                        const int* arr = std::get<std::unique_ptr<int[]>>(message).get();
                        std::cout << "Message Type: int[]" << std::endl;
                        for (int i = 0; i < *size; ++i) {
                            std::cout << arr[i] << " ";
                        }
                        std::cout << std::endl;

                    } 
                    else if (std::holds_alternative<std::unique_ptr<double[]>>(message)) {
                        const double* arr = std::get<std::unique_ptr<double[]>>(message).get();
                        std::cout << "Message Type: double[]" << std::endl;
                        for (int i = 0; i < *size; ++i) {
                            std::cout << arr[i] << " ";
                        }
                        std::cout << std::endl;
                    }
                }
        };

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
                // Test::testClientSocket(clientSocket);

                char clientIP[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

                std::cout << "Client from " << clientIP << " on port " << clientSocket << std::endl;

                clientInteraction(clientSocket);
            }
        }

        void clientInteraction(const int clientSocket) {
            // welcome message
            const char message[] = "Connection established with server, please write your name to identify you by";
            auto charArray = std::make_unique<char[]>(sizeof(message));
            memcpy(charArray.get(), message, sizeof(message));

            Message newMessage(CHAR_ARR, sizeof(message), std::move(charArray));

            sendMessageToClient(clientSocket, newMessage);
        }

        
        void sendMessageToClient(const int clientSocket, Message &newMessage) {
            // type of message being sent
            const auto messageType = newMessage.getType();
            int send1 = send(clientSocket, &messageType, sizeof(messageType), 0);
            // Test::testSending(send1, sizeof(messageType));

            // length prefix protocol (send size of data first then send the data)
            const auto sizeOfMessage = newMessage.getSize();
            int send2 = send(clientSocket, &sizeOfMessage, sizeof(sizeOfMessage), 0);
            // Test::testSending(send2, sizeof(sizeOfMessage));

            // seinding the welcom message
            sendMessageItself(clientSocket, newMessage, messageType, sizeOfMessage);
            
            std::cout << "Message sent to client from server" << std::endl;
        }

        template <typename messageType>
        void sendMessageItself(const int clientSocket, const messageType &message, const int type, const int size) {
            int send3 = INT8_MIN;
            if (type == INT_ARR) {
                send3 = send(clientSocket, std::get<std::unique_ptr<int[]>>(message.getMessage()).get(), size, 0);
            }
            else if (type == CHAR_ARR) {
                send3 = send(clientSocket, std::get<std::unique_ptr<char[]>>(message.getMessage()).get(), size, 0);  
            }
            else if (type == DOUBLE_ARR) {
                send3 = send(clientSocket, std::get<std::unique_ptr<double[]>>(message.getMessage()).get(), size, 0);
            }
            else if (type == INT) {
                send3 = send(clientSocket, std::get<std::unique_ptr<int>>(message.getMessage()).get(), size, 0);
            }
            else if (type == CHAR) {
                send3 = send(clientSocket, std::get<std::unique_ptr<char>>(message.getMessage()).get(), size, 0);
            }
            else if (type == DOUBLE) {
                send3 = send(clientSocket, std::get<std::unique_ptr<double>>(message.getMessage()).get(), size, 0);
            }
            // Test::testSending(send3, sizeof(send3));
        }

        // template <typename functionReturnType>
        // functionReturnType receiveMessageFromClient(const int clientSocket) {
        //     // create new message
        //     Message newMessage;

        //     // get type
        //     int type; 
        //     int recv1 = recv(clientSocket, &type, sizeof(type), 0);
        //     Test::testSending(recv1, 4);  

        //     if(type == 0) {
        //         Message<int []> newMessage;

        //         // get size
        //         int size;
        //         int recv2 = recv(clientSocket, &size, sizeof(size), 0);
        //         Test::testSending(recv2, 4);
        //         newMessage.setSize(size);
                
        //         // get message
        //         int recivedMessage[size];
        //         int recv3 = recv(clientSocket, recivedMessage, sizeof(recivedMessage), 0);
        //         Test::testSending(recv3, sizeof(recivedMessage));
        //         newMessage.setMessage(recivedMessage);
        //         return newMessage;
        //     }
        //     else if (type == 1) {
        //         int size;
        //         int recv2 = recv(clientSocket, &size, sizeof(size), 0);
        //         Test::testSending(recv2, 4);
        //         newMessage.setSize(size);
        //         char recivedMessage[size];
        //         int recv3 = recv(clientSocket, recivedMessage, sizeof(recivedMessage), 0);
        //         Test::testSending(recv3, sizeof(recivedMessage));
        //         newMessage.setMessage(recivedMessage);
        //         return newMessage;
        //     }
        // }

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
