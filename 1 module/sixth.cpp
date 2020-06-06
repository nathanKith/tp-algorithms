#include <iostream>

template <typename Comparator = std::greater<int>>
int median_three(int* arr, int v1, int v2, int v3, Comparator cmp = Comparator()) {
   if (cmp(arr[v1], arr[v2])) {
       std::swap(v1, v2);
   }
   if (cmp(arr[v2], arr[v3])) {
       std::swap(v2, v3);
   }
   return v2;
}

template <typename Comparator = std::greater_equal<int>>
int partition (int* arr, int left, int right, Comparator cmp = Comparator())
{
   int j = right;
   int pivot = median_three(arr, left, (left + right) / 2, right);
   std::swap(arr[left], arr[pivot]); // put pivot to start
   for (int i = right; i > left; i--)
   {
       if (cmp(arr[i], arr[left]))
       {
           std::swap(arr[i], arr[j--]);
       }
   }
   std::swap(arr[left], arr[j]); // return pivot to its place
   return j;
}

template <typename Comparator = std::greater_equal<int>>
int kth_statistic(int* arr, int left, int right, int k, Comparator cmp = Comparator()) {
   int pivot_index = 0;
   while ((pivot_index = partition(arr, left, right, cmp)) != k) {
       if (pivot_index < k) {
           left = pivot_index + 1;
       } else  {
           right = pivot_index;
       }
   }
   return arr[pivot_index];
}

int main() {
   int n, k;
   std::cin >> n >> k;
   int* a = new int[n];
   for (int i = 0; i < n; ++i) {
       std::cin >> a[i];
   }
   std::cout << kth_statistic(a, 0, n - 1, k);
   return 0;
}
