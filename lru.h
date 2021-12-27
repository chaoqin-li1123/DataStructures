#ifndef LRU_H
#define LRU_H

#include <list>
#include <unordered_map>

template <typename Key, typename Value> class LRUCache {
public:
  LRUCache(int capacity) : capacity_{capacity} {}

  Value get(Key key) {
    auto it = dict.find(key);
    if (it == dict.end()) {
      return {};
    }
    const auto& [k, v] = *(it->second);
    cache.splice(cache.end(), cache, it->second);
    return v;
  }

  void put(Key key, Value value) {
    auto it = dict.find(key);
    if (it != dict.end()) {
      cache.erase(it->second);
    }
    cache.push_back({key, value});
    dict[key] = (--cache.end());
    cleanUp();
  }

  void cleanUp() {
    while (cache.size() > capacity_) {
      const auto& [k, v] = *cache.begin();
      dict.erase(k);
      cache.pop_front();
    }
  }

private:
  std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator>
      dict;
  std::list<std::pair<Key, Value>> cache;
  const int capacity_;
};

#endif