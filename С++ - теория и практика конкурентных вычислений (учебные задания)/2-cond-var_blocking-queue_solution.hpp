#pragma once

#include <tpcc/stdlike/condition_variable.hpp>

#include <cstddef>
#include <deque>
#include <mutex>
#include <stdexcept>

namespace tpcc {
namespace solutions {

class QueueClosed : public std::runtime_error {
 public:
  QueueClosed() : std::runtime_error("Queue closed for Puts") {
  }
};

template <typename T, class Container = std::deque<T>>
class BlockingQueue {
 public:
  // capacity == 0 means queue is unbounded
  explicit BlockingQueue(const size_t capacity = 0) : capacity_(capacity) {
  }

  // throws QueueClosed exception after Close
  void Put(T item) {
    std::unique_lock<std::mutex> lock(MutexQueue);
    if (closed_) {
      throw QueueClosed();
      return;
    }
    while (items_.size() == capacity_) {
      IsNotFull.wait(lock);
      if (closed_) {
        throw QueueClosed();
        return;
      }
    }
    items_.push_back(std::move(item));
    IsNotEmpty.notify_one();
  }

  // returns false iff queue is empty and closed
  bool Get(T& item) {
    std::unique_lock<std::mutex> lock(MutexQueue);
    if (closed_ && items_.empty()) {
      return false;  // to be implemented
    }
    while (items_.empty()) {
      IsNotEmpty.wait(lock);
      if (closed_) {
        return false;
      }
    }
    item = std::move(items_.front());
    items_.pop_front();
    IsNotFull.notify_one();
    return true;
  }

  // close queue for Puts
  void Close() {
    std::unique_lock<std::mutex> lock(MutexQueue);
    closed_ = true;
    IsNotEmpty.notify_all();
    IsNotFull.notify_all();
  }

 private:
  // internal predicates for condition variables

  bool IsFull() const {
    // to be implemented
  }

  bool IsEmpty() const {
    // to be implemented
  }

 private:
  size_t capacity_;
  Container items_;
  bool closed_{false};

  // use tpcc::condition_variable-s
  std::mutex MutexQueue;
  std::condition_variable IsNotEmpty;
  std::condition_variable IsNotFull;
};

}  // namespace solutions
}  // namespace tpcc
