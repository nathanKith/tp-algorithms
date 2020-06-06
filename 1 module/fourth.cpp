/**
* Вовочка ест фрукты из бабушкиной корзины.В корзине лежат фрукты разной массы.
* Вовочка может поднять не более K грамм. Каждый фрукт весит не более K грамм.
* За раз он выбирает несколько самых тяжелых фруктов, которые может поднять одновременно, о
* ткусывает от каждого половину и кладет огрызки обратно в корзину. Если фрукт весит нечетное число грамм,
* он откусывает большую половину. Фрукт массы 1гр он съедает полностью.
* Определить за сколько подходов Вовочка съест все фрукты в корзине.
*/
#include <iostream>
#include <assert.h>
#include <cstring>
#include <vector>

template<class T, typename Comparator = std::greater<T>>
class Heap {
public:
   Heap() : arrSize(0), memSize(8) {
       arr = new T[memSize];
   }

   explicit Heap(const T *mas, int size)
           : arrSize(size), memSize(size * 2) {
       arr = new T[memSize];
       std::memcpy(arr, mas, size * sizeof(T));
       buildHeap();
   }

   ~Heap() {
       delete[]arr;
   }

   void Insert(T element) {
       if (arrSize == memSize) {
           upSize();
       }
       arr[arrSize++] = element;
       siftUp(arrSize - 1);
   }

   T Extract() {
       if (arrSize == 0) {
           return -1;
       }
       T result = arr[0];
       arr[0] = arr[--arrSize];
       if (arrSize != 0) {
           siftDown(0);
       }
       return result;
   }

   T Peek() const {
       if (arrSize == 0) {
           return -1;
       }
       return arr[0];
   }

   void Print() {
       for (int i = 0; i < arrSize; ++i) {
           std::cout << arr[i] << "  ";
       }
   }

private:
   T *arr;
   int memSize;
   int arrSize;

   void buildHeap() {
       for (int i = arrSize / 2 - 1; i >= 0; --i) {
           siftDown(i);
       }
   }

   void upSize() {
       memSize *= 2;
       T *buffer = new T[memSize];
       std::memcpy(buffer, arr, arrSize * sizeof(T));
       delete[]arr;
       arr = buffer;
   }

   void siftDown(int index, Comparator cmp = Comparator()) {
       int left = 2 * index + 1;
       int right = 2 * index + 2;

       int largest = index;
       if (left < arrSize && cmp(arr[left], arr[index])) {
           largest = left;
       }
       if (right < arrSize && cmp(arr[right], arr[largest])) {
           largest = right;
       }

       if (largest != index) {
           std::swap(arr[index], arr[largest]);
           siftDown(largest);
       }
   }

   void siftUp(int index, Comparator cmp = Comparator()) {
       while (index > 0) {
           int parent = (index - 1) / 2;
           if (!cmp(arr[index], arr[parent])) {
               return;
           }
           std::swap(arr[index], arr[parent]);
           index = parent;
       }
   }
};

int main() {
   int n = 10;
   Heap<int, std::greater<int>> fruitHeap;
   int *fruits = new int[n];
   for (int i = 0; i < n; ++i) {
       std::cin >> fruits[i];
       fruitHeap.Insert(fruits[i]);
   }

   int steps = 0;
   int totalWeight = 0;
   fruitHeap.Print();
   return 0;
}