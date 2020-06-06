/* Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
* Для каждого элемента массива B[i] найдите минимальный индекс элемента массива A[k], ближайшего по значению к B[i].
* Требования: Время работы поиска для каждого элемента B[i]: O(log(k)). Внимание!
* В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k,
* а потом уже в нем делать бинарный поиск.
**/

#include <iostream>

int binary_search(int value, int* mas, int left, int right);
void find_borders(int* mas, int n, int value, int& left, int& right);

int main() {
   int n = 0;
   std::cin >> n;
   int* a = new int[n];
   for (int i = 0; i < n; ++i) {
       std::cin >> a[i];
   }

   int m = 0;
   std::cin >> m;
   int* b = new int[m];
   for (int i = 0; i < m; ++i) {
       std::cin >> b[i];
   }

   for (int i = 0; i < m; ++i) {
       int left = 0, right = 0;
       find_borders(a, n, b[i], left, right);
       std::cout << binary_search(b[i], a, left, right) << ' ';
   }

   delete [] a;
   delete [] b;
   return 0;
}

int binary_search(int value, int* mas, int left, int right) {
   if (left == right) {
       return left;
   }
   int middle = 0;
   while (right - left > 1) {
       middle = (left + right) / 2;
       if (value <= mas[middle]) {
           right = middle;
       } else {
           left = middle;
       }
   }
   if (left == right && left > 0) {
       left--;
   }
   if (value - mas[left] <= mas[right] - value) {
       return left;
   }
   return right;
}

void find_borders(int* mas, int n, int value, int& left, int& right)
{
   if (value < mas[0]) {
       left = 0;
       right = 0;
       return;
   }
   if (value > mas[n - 1]) {
       left = n - 1;
       right = n - 1;
       return;
   }
   int k = 1;
   for (; mas[k] < value && k < n; k *= 2) {}
   if (k < n) {
       right = k;
   } else {
       right = n - 1;
   }
   if (k == 1) {
       left = 0;
   } else {
       left = k / 2;
   }
}
