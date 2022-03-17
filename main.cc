#include "circular_buffer.h"
#include "consistent_hash.h"
#include "lru.h"
#include <iostream>

void TestConsistentHash() {
  RingHash consistent_hash;
  consistent_hash.insert("aaa", 1000);
  consistent_hash.insert("abs", 10);
  consistent_hash.remove("abs");
  consistent_hash.insert("abs", 1000);
  std::cout << consistent_hash.get("fdqwfd") << std::endl;
}

void TestLRUCache() {
  LRUCache<int, int> cache(10);
  for (int i = 0; i < 20; i++)
    cache.put(i, i + 3);
  std::cout << cache.get(1) << std::endl;
  std::cout << cache.get(15) << std::endl;
}

void TestMaglevHash() {
  MaglevHash maglev_hash({"a", "b", "c"}, {5, 4, 3});
  int f[3]{};
  for (int i = 0; i < 200; i++) {
    f[maglev_hash.get(std::to_string(i))[0] - 'a']++;
  }
  for (int i = 0; i < 3; i++) {
    std::cout << f[i] << std::endl;
  }
}

void TestCircularBuffer() {
  CircularBuffer<int> buffer(10);
  for (int i = 0; i < 10; i++) {
    buffer.push_back(i);
  }
  for (int i = 0; i < 5; i++) {
    buffer.pop_front();
  }
  for (int i = 14; i < 18; i++) {
    buffer.push_back(i);
  }
  while (!buffer.empty()) {
    std::cerr << buffer.front() << ";";
    buffer.pop_front();
  }
}

int main(int argc, char *argv[]) {
  TestCircularBuffer();
  return 0;
}