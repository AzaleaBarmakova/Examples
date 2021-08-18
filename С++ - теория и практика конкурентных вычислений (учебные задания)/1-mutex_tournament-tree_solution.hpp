#pragma once

#include <math.h>

#include <atomic.hpp>
#include <mutex>
#include <support.hpp>
#include <thread>
#include <vector>

#include <algorithm>
#include <iostream>

using namespace std;

namespace tpcc {
namespace solutions {

class PetersonLock {
 public:
  PetersonLock() {
    want_[0].store(false);
    want_[1].store(false);
    victim_.store(0);
  }

  void Lock(size_t thread_index) {
    want_[thread_index].store(true);
    victim_.store(thread_index);
    while (want_[1 - thread_index].load() && victim_.load() == thread_index) {
      std::this_thread::yield();
    }
    return;
  }

  void Unlock(size_t thread_index) {
    want_[thread_index].store(false);
    return;
  }

 private:
  PetersonLock(const PetersonLock&);

  array<atomic<bool>, 2> want_;
  atomic<size_t> victim_;
};

class TournamentTreeLock {
 public:
  explicit TournamentTreeLock(size_t num_threads) {
    height_ = max(1, static_cast<int>(ceil(log2(num_threads))));
    num_tree_nodes_ = pow(2, height_) - 1;
    mutex_tree_ = vector<PetersonLock>(num_tree_nodes_);
  }

  void Lock(size_t thread_index) {
    size_t winner = thread_index + num_tree_nodes_;
    for (size_t i = 0; i < height_; ++i) {
      int now_locked = 1 - (winner % 2);
      winner = (winner - 1) / 2;
      mutex_tree_[winner].Lock(now_locked);
    }
    return;
  }

  void Unlock(size_t thread_index) {
    size_t index = 0;
    for (int i = height_ - 1; i >= 0; --i) {
      mutex_tree_[index].Unlock((thread_index >> i) % 2);
      index = index * 2 + 1 + (thread_index >> i) % 2;
    }
    return;
  }

 private:
  // Tree navigation

  size_t GetParent(size_t node_index) const {
    return node_index / 2;
  }

  size_t GetLeftChild(size_t node_index) const {
    return node_index * 2;
  }

  size_t GetRightChild(size_t node_index) const {
    return node_index * 2 + 1;
  }

  size_t GetThreadLeaf(size_t thread_index) const {
    return thread_index + num_tree_nodes_;
  }

 private:
  double height_;
  size_t num_tree_nodes_;
  std::mutex mutex_;
  vector<PetersonLock> mutex_tree_;
};

}  // namespace solutions
}  // namespace tpcc
