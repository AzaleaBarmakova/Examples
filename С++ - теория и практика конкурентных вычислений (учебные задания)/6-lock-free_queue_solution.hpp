#pragma once

#include <tpcc/stdlike/atomic.hpp>
#include <tpcc/support/compiler.hpp>

#include <utility>

namespace tpcc {
namespace solutions {

// Michael-Scott lock-free queue

template <typename T>
class LockFreeQueue {
  struct Node {
    T item_;
    tpcc::atomic<Node*> next_{nullptr};

    Node() {
    }

    explicit Node(T item, Node* next = nullptr)
        : item_(std::move(item)), next_(next) {
    }
  };

  void help() {
    Node* tail = tail_.load();
    if (tail->next_.load() &&
        tail_.compare_exchange_strong(tail, tail->next_.load()))
      help();
  }

  Node* next(Node* node) {
    if (node)
      return node->next_.load();
    else
      return NULL;
  }

  Node* Pop(tpcc::atomic<Node*>& head) {
    Node* node = NULL;
    while (!head.compare_exchange_weak(node, next(node)))
      ;
    return node;
  }

 public:
  LockFreeQueue() {
    Node* dummy = new Node{};
    head_ = dummy;
    tail_ = dummy;
    first_ = dummy;
  }

  ~LockFreeQueue() {
    Node* node;
    while (node = Pop(first_)) delete node;
  }

  void Enqueue(T item) {
    Node* newNode = new Node(item);
    Node* nullNode;
    do {
      nullNode = nullptr;
      help();
    } while (!tail_.load()->next_.compare_exchange_weak(nullNode, newNode));
    help();
  }

  bool Dequeue(T& item) {
    Node* curHead;
    do {
      curHead = head_.load();
      if (curHead == tail_.load() || curHead->next_.load() == nullptr)
        return false;
    } while (!head_.compare_exchange_weak(curHead, curHead->next_.load()));
    item = std::move(curHead->next_.load()->item_);
    return true;
  }

 private:
  tpcc::atomic<Node*> head_{nullptr};
  tpcc::atomic<Node*> tail_{nullptr};
  tpcc::atomic<Node*> first_{nullptr};
};

}  // namespace solutions
}  // namespace tpcc
