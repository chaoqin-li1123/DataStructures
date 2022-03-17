#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <cstddef>
#include <cstdlib>

template <typename T> class CircularBuffer {
public:
  CircularBuffer(size_t capacity) : capacity_(capacity) {
    buffer_ = (T *)malloc(capacity_ * sizeof(T));
  }
  bool push_back(T &data) {
    if (size_ == capacity_){
      return false;
    }
    set(back_idx(), data);
    size_++;
    return true;
  }
  bool push_front(T &data) {
    if (size_ == capacity_){
      return false;
    }
    start_ = prev(start_);
    size_++;
    set(start_, data);
  }
  void pop_front() {
    if (size_ > 0) {
      buffer_[start_].~T();
      start_ = next(start_);
      size_--;
    }
  }
  void pop_back() {
    if (size_ > 0) {
      buffer_[back_idx()].~T();
      size_--;
    }
  }
  T &front() { return buffer_[start_]; }
  T &back() { return buffer_[back_idx()]; }
  ~CircularBuffer() {
    while (!empty()) {
      pop_front();
    }
    free((void *)buffer_);
  }
  bool empty() {
    return size_ == 0;
  }
  size_t size() {
    return size_;
  }

private:
  void set(size_t index, T &data) { new (buffer_ + index) T(data); }
  size_t back_idx() const { return (start_ + size_) % capacity_; }
  size_t next(size_t i) { return (i + 1) % capacity_; }
  size_t prev(size_t i) { return (i + capacity_ - 1) % capacity_; }

  size_t capacity_;
  size_t start_{0};
  size_t size_{0};
  T *buffer_;
};

#endif