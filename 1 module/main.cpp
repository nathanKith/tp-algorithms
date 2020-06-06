#include <iostream>

int count(int n, int* a, int m, int*b, int k);

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
   int k = 0;
   std::cin >> k;
   std::cout << count(n, a, m, b, k);
   delete []a;
   delete []b;
   return 0;
}

int count(int n, int* a, int m, int* b, int k) {
   int count = 0, j = 0;
   for (int i = m - 1; i >= 0; --i) {
       for (; j < n; ++j) {
           if (a[j] + b[i] == k) {
               ++count;
           }
           if (a[j] + b[i] > k) {
               break;
           }
       }
   }
   return count;
}
