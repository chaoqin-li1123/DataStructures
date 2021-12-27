#ifndef CONSISTENT_HASH_H
#define CONSISTENT_HASH_H

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class ConsistentHash {
public:
  // Add a node to the set.
  void insert(const std::string &node, const int vnodes_cnt) {
    remove(node);
    n_vnodes_[node] = vnodes_cnt;
    for (int i = 0; i < vnodes_cnt; i++) {
      size_t vnode = std::hash<std::string>{}(node + std::to_string(i));
      std::vector<std::string> &nodes = virtual2real_[vnode];
      if (nodes.empty())
        vnodes_.insert(vnode);
      nodes.push_back(node);
    }
  }

  // Remove the node from the set.
  void remove(const std::string &node) {
    for (int i = 0; i < n_vnodes_[node]; i++) {
      size_t vnode = std::hash<std::string>{}(node + std::to_string(i));
      std::vector<std::string> &nodes = virtual2real_[vnode];
      for (auto it = nodes.begin(); it != nodes.end(); it++) {
        if (*it == node) {
          nodes.erase(it);
          break;
        }
      }
      if (nodes.empty())
        vnodes_.erase(vnode);
    }
    n_vnodes_.erase(node);
  }

  // Look up a node with key.
  std::string get(const std::string &key) {
    if (vnodes_.empty())
      return std::string();
    size_t key_hash = std::hash<std::string>{}(key);
    auto vnode_it = vnodes_.lower_bound(key_hash);
    if (vnode_it == vnodes_.end())
      vnode_it = vnodes_.begin();
    auto vnode = *vnode_it;
    if (virtual2real_.find(vnode) == virtual2real_.end())
      return std::string();
    std::vector<std::string> const &nodes = virtual2real_[vnode];
    return nodes[key_hash % nodes.size()];
  }

private:
  std::unordered_map<size_t, std::vector<std::string>> virtual2real_;
  std::unordered_map<std::string, int> n_vnodes_;
  std::set<size_t> vnodes_;
};

#endif