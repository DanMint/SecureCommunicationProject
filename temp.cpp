#include <iostream>
#include <vector>

class MyCircularQueue {
    private:   
       std::vector<int*> circularQueue;
       int start;
       int end;
       int size;
       

    public:
        MyCircularQueue(int k) {
            size = k;
            start = -1;
            end = -1;

            for (int i = 0; i < size; i ++) 
                circularQueue.push_back(nullptr);
        }
        
        bool enQueue(int value) {
            if (!isFull()) {
                if (start == -1 && end == -1) 
                    start = 0;
                    
                if (end <= size - 2 && circularQueue[end + 1] == nullptr) {
                    end  += 1;
                    circularQueue[end] = new int(value);
                }
                else {
                    if (circularQueue[0] == nullptr) {
                        end  = 0;
                        circularQueue[end] = new int(value);
                    }
                }
                return true;
            }
            return false;
        }
        
        bool deQueue() {
            if (!isEmpty()) {
                if (start <= size - 2) {
                    circularQueue[start] = nullptr;
                    start += 1;
                }
                else {
                    circularQueue[start] = nullptr;
                    start = 0;
                }
                return true;
            }

            return false;
        }
        
        int Front() {
            if (circularQueue[start] == nullptr)
                return -1;

            return *circularQueue[start];
        }
        
        int Rear() {
            if (circularQueue[end] == nullptr)
                return -1;

            return *circularQueue[end];
        }
        
        bool isEmpty() {
            for (const auto &el : circularQueue) {
                if (el != nullptr)
                    return false;
            }

            return true;
        }
        
        bool isFull() {
            for (const auto &el : circularQueue) {
                if (el == nullptr)
                    return false;
            }

            return true;
        }

        void printQueue() {
            for (const auto &el : circularQueue) {
                if (el == nullptr)
                    continue;

                std::cout << *el << " ";
            }
            std::cout << std::endl;
        }
};

int main() {
    MyCircularQueue temp(3);

    std::cout << temp.Rear() << std::endl;
    std::cout << temp.enQueue(2) << std::endl;
    std::cout << temp.enQueue(3) << std::endl;
    std::cout << temp.enQueue(4) << std::endl;
    std::cout << temp.Rear() << std::endl;
    std::cout << temp.isFull() << std::endl;
    std::cout << temp.deQueue() << std::endl;
    std::cout << temp.enQueue(4) << std::endl;

    return 0;
}