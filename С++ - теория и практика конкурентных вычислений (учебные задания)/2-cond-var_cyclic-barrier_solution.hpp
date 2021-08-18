#pragma once

#include <tpcc/stdlike/condition_variable.hpp>

#include <mutex>
#include <cstddef>

namespace tpcc {
namespace solutions {

class CyclicBarrier {
 public:
  explicit CyclicBarrier(const size_t num_threads)
      : num_threads_(num_threads),
        num_threads_arrived_(0),
        all_threads_wait_(false) {
  }

  void PassThrough() {
    // to be implemented
    std::unique_lock<std::mutex> lock(mutex_);
    while (all_threads_wait_) {
      all_threads_arrived_.wait(lock);
    }

    num_threads_arrived_++;
    if (num_threads_arrived_ == num_threads_) {
      all_threads_wait_ = true;
    }

    while (num_threads_arrived_ != num_threads_ && !all_threads_wait_) {
      all_threads_arrived_.wait(lock);
    }

    if (num_threads_arrived_ == num_threads_ && all_threads_wait_) {
      all_threads_arrived_.notify_all();
    }
    num_threads_arrived_--;

    if (num_threads_arrived_ == 0) {
      all_threads_wait_ = false;
      all_threads_arrived_.notify_all();
    }
  }

 private:
  std::mutex mutex_;
  std::condition_variable all_threads_arrived_;
  size_t num_threads_;
  size_t num_threads_arrived_;
  bool all_threads_wait_;
};

}  // namespace solutions
}  // namespace tpcc
