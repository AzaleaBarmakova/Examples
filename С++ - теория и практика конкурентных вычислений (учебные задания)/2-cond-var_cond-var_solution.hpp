#pragma once

#include <tpcc/concurrency/futex.hpp>

#include <atomic>
#include <limits.h>
#include <linux/futex.h>
#include <syscall.h>
#include <unistd.h>

namespace tpcc {
namespace solutions {

class ConditionVariable {
 public:
  ConditionVariable() : futex_(signal_count_) {
  }

  template <class Mutex>
  void Wait(Mutex& mutex) {
    // use mutex.unlock() / mutex.lock() to unlock/lock mutex
    old_signal_ = signal_count_;
    mutex.unlock();
    futex__wait(signal_count_, old_signal_);
    mutex.lock();
  }

  void NotifyOne() {
    fetch_add(signal_count_, 1);
    futex__wake(signal_count_, 1);
  }

  void NotifyAll() {
    signal_count_ = 0;
    futex__wake(signal_count_, INT_MAX);
  }

 private:
  std::atomic<uint32_t> signal_count_{0};
  std::atomic<uint32_t> old_signal_{0};
  tpcc::Futex futex_;
};

}  // namespace solutions
}  // namespace tpcc
