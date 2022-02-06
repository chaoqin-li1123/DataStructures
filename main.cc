#include "consistent_hash.h"
#include "lru.h"
#include <iostream>

int main(int argc, char* argv[]) {
    /*
    RingHash consistent_hash;
    consistent_hash.insert("aaa", 1000);
    consistent_hash.insert("abs", 10);
    consistent_hash.remove("abs");
    consistent_hash.insert("abs", 1000);
    std::cout << consistent_hash.get("fdqwfd") << std::endl; 
    */

    /*
    LRUCache<int, int> cache(10);
    for (int i = 0; i < 20; i++) cache.put(i, i + 3);
    std::cout << cache.get(1) << std::endl; 
    std::cout << cache.get(15) << std::endl; 
    */
    MaglevHash maglev_hash({"a", "b", "c"}, {5, 4, 3});
    int f[3]{};
    for (int i = 0; i < 200; i++) {
        f[maglev_hash.get(std::to_string(i))[0] - 'a']++;
    }
    for (int i = 0; i < 3; i++) {
        std::cout << f[i] << std::endl;
    }   
    return 0;
}