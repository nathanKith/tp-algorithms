#include <iostream>
#include <vector>
#include <signal.h>

class StringHasher {
public:
   StringHasher(size_t prime = 71): prime(prime) {}
   size_t operator()(const std::string &str, size_t sz) const {
       size_t hash = 0;
       for (char i : str) {
           hash = (hash * prime + i) % sz;
       }
       return hash;
   }
private:
   size_t prime;
};

class HashTable {
public:
   HashTable() : currentSize(0) {
       buffer.resize(8, "__nil__");
   }

   ~HashTable() = default;

   bool Add(const std::string& key) {
       // коэффициенты 3 и 4 получены из пропорции currentSize/buffer.Size() >= 3/4
       if (4 * currentSize >= 3 * buffer.size()) {
           grow();
       }

       if (Has(key)) {
           return false;
       }

       size_t index = 0, hashIndex = squareProb(key, index, buffer.size());
       while (buffer[hashIndex] != "__nil__" && buffer[hashIndex] != "__del__") {
           hashIndex = squareProb(key, ++index, buffer.size());
       }
       buffer[hashIndex] = key;

       ++currentSize;
       return true;
   }

   bool Delete(const std::string& key) {
       if (!Has(key)) {
           return false;
       }
       size_t index = 0, hashIndex = squareProb(key, index, buffer.size());
       while (buffer[hashIndex] != "__nil__" && buffer[hashIndex] != "__del__") {
           if (buffer[hashIndex] == key) {
               buffer[hashIndex] = "__del__";
               currentSize--;
               break;
           }
           hashIndex = squareProb(key, ++index, buffer.size());
       }
       return true;
   }

   bool Has(const std::string& key) {
       size_t index = 0, hashIndex = squareProb(key, index, buffer.size());
       while (buffer[hashIndex] != "__nil__" && index != buffer.size() - 1) {
           if (buffer[hashIndex] == key) {
               return true;
           }
           hashIndex = squareProb(key, ++index, buffer.size());
       }
       return false;
   }

private:
   std::vector<std::string> buffer;
   size_t currentSize;
   StringHasher hashing;

   void grow() {
       std::vector<std::string> copyBuffer(buffer.size() * 2, "__nil__");
       for (const auto& key : buffer) {
           if (key == "__nil__") {
               continue;
           }
           size_t index = 0, hashIndex = squareProb(key, index, copyBuffer.size());
           while (copyBuffer[hashIndex] != "__nil__" && copyBuffer[hashIndex] != "__del__") {
               hashIndex = squareProb(key, ++index, copyBuffer.size());
           }
           copyBuffer[hashIndex] = key;
       }
       buffer.resize(copyBuffer.size(), "__nil__");
       std::copy(copyBuffer.begin(), copyBuffer.end(), buffer.begin());
   }

//    size_t squareProb(const std::string& key, size_t index, size_t sz) {
//        if (index == 0) {
//            return hashing(key, sz);
//        }
//        return squareProb(key, index - 1, sz) + index + 1 % sz;
//    }

   size_t squareProb(const std::string& key, size_t index, size_t sz) {
       return (hashing(key, sz) + index / 2 + (index * index) / 2) % sz;
   }
};

int main() {
   HashTable table;
   char op;
   std::string str;
   int i = 1;
   while (std::cin >> op >> str)
   {
       switch (op)
       {
           case '?':
           {
               std::cout << (table.Has(str) ? "OK" : "FAIL") << std::endl;
               break;
           }
           case '+':
           {
               if (i == 11) {
                   std::cout << (table.Add(str) ? "OK" : "FAIL") << std::endl;
                   break;
               }
               else {
                   std::cout << (table.Add(str) ? "OK" : "FAIL") << std::endl;
                   break;
               }
           }
           case '-':
           {
               std::cout << (table.Delete(str) ? "OK" : "FAIL") << std::endl;
               break;
           }
       }
       ++i;
   }
   return 0;
}