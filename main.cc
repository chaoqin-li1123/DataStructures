#include "consistent_hash.h"
#include "lru.h"
#include <iostream>

int main(int argc, char* argv[]) {
    
    ConsistentHash consistent_hash;
    consistent_hash.insert("aaa", 1000);
    consistent_hash.insert("abs", 10);
    consistent_hash.remove("abs");
    consistent_hash.insert("abs", 1000);
    std::cout << consistent_hash.get("fdqwfd") << std::endl; 

    LRUCache<int, int> cache(10);
    for (int i = 0; i < 20; i++) cache.put(i, i + 3);
    std::cout << cache.get(1) << std::endl; 
    std::cout << cache.get(15) << std::endl; 
    return 0;
}