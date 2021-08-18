#pragma once

#include <rwlock_traits.hpp>

#include <tpcc/stdlike/condition_variable.hpp>

#include <mutex>

namespace tpcc {
namespace solutions {

class ReaderWriterLock {
 public:
  int RL = 0;
  int WL = 0;
  std::mutex mutex_;
  std::condition_variable cv_;
  // reader section / shared ownership

  void ReaderLock() {
    // to be implemented
    std::unique_lock<std::mutex> lock(mutex_);
    while (WL > 0) {
      cv_.wait(lock);
    }
    RL++;
  }

  void ReaderUnlock() {
    // to be implemented
    std::unique_lock<std::mutex> lock(mutex_);
    RL--;
    cv_.notify_one();
  }

  // writer section / exclusive ownership

  void WriterLock() {
    // to be implemented
    std::unique_lock<std::mutex> lock(mutex_);
    WL++;
    while (RL > 0) {
      cv_.wait(lock);
    }
  }

  void WriterUnlock() {
    // to be implemented
    std::unique_lock<std::mutex> lock(mutex_);
    WL--;
    cv_.notify_all();
  }
};

}  // namespace solutions
}  // namespace tpcc
