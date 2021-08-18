#pragma once

#include <tpcc/stdlike/condition_variable.hpp>

namespace tpcc {
namespace solutions {

class Semaphore {
 public:
  explicit Semaphore(const size_t count = 0) : count_(count) {
  }

  void Acquire() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (count_ == 0) {
      has_tokens_.wait(lock);
    }
    count_--;
  }

  void Release() {
    std::unique_lock<std::mutex> lock(mutex_);
    count_++;
    has_tokens_.notify_one();
  }

 private:
  size_t count_;
  std::mutex mutex_;
  tpcc::condition_variable has_tokens_;
};

}  // namespace solutions
}  // namespace tpcc
