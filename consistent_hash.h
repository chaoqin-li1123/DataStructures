#ifndef CONSISTENT_HASH_H
#define CONSISTENT_HASH_H

#include <algorithm>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class RingHash {
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

class MaglevHash {
public:
  MaglevHash(std::vector<std::string> const &hosts,
             std::vector<size_t> const &weights)
      : hosts_(hosts), turn_(weights) {
    build();
  }
  void build() {
    std::vector<TableBuildEntry> build_entries;
    build_entries.reserve(hosts_.size());
    for (auto i = 0; i < hosts_.size(); i++) {
      build_entries.emplace_back(
          std::hash<std::string>{}(hosts_[i]) % TABLE_SIZE,
          std::hash<std::string>{}(hosts_[i]) % (TABLE_SIZE - 1) + 1);
    }

    size_t filled = 0;
    while (true) {
      turn_.iterate();
      for (size_t i = 0; i < hosts_.size(); i++) {
        if (!turn_.tryPick(i))
          continue;
        while (!table_[build_entries[i].permutation()].empty()) {
          build_entries[i].next_++;
        }
        table_[build_entries[i].permutation()] = hosts_[i];
        filled++;
        if (filled == TABLE_SIZE)
          return;
      }
    }
  }
  std::string get(std::string key) {
    return table_[std::hash<std::string>{}(key) % TABLE_SIZE];
  }

private:
  struct TableBuildEntry {
    TableBuildEntry(size_t offset, size_t skip)
        : offset_(offset), skip_(skip) {}
    size_t permutation() {
      return (offset_ + (skip_ * next_)) % MaglevHash::TABLE_SIZE;
    }
    const size_t offset_;
    const size_t skip_;
    size_t next_{0};
  };
  struct WeightedTurnPicker {
    WeightedTurnPicker(std::vector<size_t> const &weights) : weights_(weights) {
      max_weight_ = *max_element(weights_.begin(), weights_.end());
      current_weights_.resize(weights_.size());
    }
    void iterate() { iteration_++; }
    bool tryPick(size_t i) {
      size_t target_weight = weights_[i] * iteration_;
      if (current_weights_[i] < target_weight) {
        current_weights_[i] += max_weight_;
        return true;
      }
      return false;
    }
    const std::vector<size_t> weights_{5, 2, 1};
    size_t max_weight_;
    std::vector<size_t> current_weights_;
    size_t iteration_{0};
  };
  static constexpr size_t TABLE_SIZE = 65537;
  std::string table_[TABLE_SIZE];
  std::vector<std::string> hosts_;
  WeightedTurnPicker turn_;
};

#endif