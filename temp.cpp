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

        MessageData getMessage() {
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
};

int main() {

    const char *message = "WUBBA BUBBA LUBBA NUBBA";
    
    int temp = sizeof(message);

    std::cout << temp << std::endl;

    

    return 0;
}