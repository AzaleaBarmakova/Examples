#pragma once

#include <tpcc/stdlike/atomic.hpp>
#include <tpcc/support/compiler.hpp>

#include <utility>

namespace tpcc {
namespace solutions {

// Treiber lock-free stack

template <typename T>
class LockFreeStack {
  struct Node {
    T item_;
    tpcc::atomic<Node*> next_{NULL};

    Node(T item) : item_(std::move(item)) {
    }
  };

  Node* Next(Node* node) {
    if (node)
      return node->next_.load();
    else
      return NULL;
  }

  void Push(Node* node, tpcc::atomic<Node*>& head) {
    Node* expected = NULL;
    do {
      node->next_.store(expected);
    } while (!head.compare_exchange_weak(expected, node));
  }

  Node* Pop(tpcc::atomic<Node*>& head) {
    Node* data = NULL;
    while (!head.compare_exchange_weak(data, Next(data))) {
    }
    return data;
  }

 public:
  LockFreeStack() {
  }

  ~LockFreeStack() {
    Node* tmpNode;
    while (tmpNode = Pop(top_)) delete tmpNode;
    while (tmpNode = Pop(deleted_)) delete tmpNode;
  }

  void Push(T item) {
    Node* data = new Node(item);
    Push(data, top_);
  }

  bool Pop(T& item) {
    Node* data = Pop(top_);
    if (!data) {
      return false;
    }
    item = std::move(data->item_);
    Push(data, deleted_);
    return true;
  }

 private:
  tpcc::atomic<Node*> top_{nullptr};
  tpcc::atomic<Node*> deleted_{nullptr};
};

}  // namespace solutions
}  // namespace tpcc
