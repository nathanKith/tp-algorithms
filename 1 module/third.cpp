#include <iostream>
#include <cstring>

class Queue {
public:
   Queue():
   memSize(5), arrSize(0), headIndex(0), tailIndex(0) {
       arr = new int[memSize];
   }

   ~Queue() {
       delete []arr;
   }
   int Pop() {
       if (isEmpty()) {
           return -1;
       }
       int res = arr[headIndex];
       if (headIndex == arrSize - 1) {
           headIndex = 0;
       } else {
           headIndex++;
       }
       arrSize--;
       return res;
   }

   void Push(int x) {
       if (memSize == arrSize) {
           upSize();
       }
       if (tailIndex == arrSize - 1) {
           tailIndex = 0;
       } else {
           ++tailIndex;
       }
       arr[tailIndex] = x;
       arrSize++;
       if (arrSize == 1) {
           headIndex = tailIndex;
       }
   }
private:
   int* arr;
   int arrSize;
   int memSize;
   int headIndex;
   int tailIndex;

   void upSize() {
       memSize *= 2;
       int* buffer = new int[memSize];
       std::memcpy(buffer, arr + sizeof(int) * headIndex, sizeof(int) * (arrSize - headIndex));
       if (tailIndex < headIndex) {
           std::memcpy(buffer + sizeof(int) * (arrSize - headIndex + 1), arr, sizeof(int) * (tailIndex + 1));
       }
       delete []arr;
       arr = buffer;
   }

   bool isEmpty() {
       return arrSize == 0;
   }

   void printBuf(int* mas) {
       std::cout << "start" << std::endl;
       for (int i = 0; i < memSize; ++i) {
           std::cout << mas[i] << ' ';
       }
       std::cout << std::endl << "end " << std::endl;
   }
};


int main() {
   int n = 0;
   std::cin >> n;
   bool flag = true;
   Queue queue;
   for (int i = 0; i < n; ++i) {
       int command, value;
       std::cin >> command >> value;
       if (command == 3) {
           queue.Push(value);
       }
       if (command == 2) {
           if (queue.Pop() != value) {
               flag = false;
           }
       }
   }
   if (flag) {
       std::cout << "YES";
   } else {
       std::cout << "NO";
   }
   return 0;
}