#pragma once

#include <tpcc/stdlike/atomic.hpp>
#include <tpcc/stdlike/condition_variable.hpp>
#include <tpcc/stdlike/mutex.hpp>

#include <tpcc/support/compiler.hpp>

#include <algorithm>
#include <forward_list>
#include <functional>
#include <shared_mutex>
#include <vector>
#include <utility>

namespace tpcc {
namespace solutions {

template <typename T, class Hash = std::hash<T>>
class StripedHashSet {
 public:
  explicit StripedHashSet(const size_t concurrency_level = 4,
                          const size_t growth_factor = 2,
                          const double max_load_factor = 0.8)
      : concurrency_level_(concurrency_level),
        growth_factor_(growth_factor),
        max_load_factor_(max_load_factor) {
    mutex_array_ = std::vector<std::shared_timed_mutex>(concurrency_level);
    hash_table_ = std::vector<std::forward_list<T>>(concurrency_level);
    element_counter_.store(0);
  }
  bool Insert(const T& element); 
  bool Remove(const T& element);
  bool Contains(const T& element); 
  
 public:
  size_t GetSize() const {
    return element_counter_.load();
  }
  
  size_t GetBucketCount() const {
    // for testing purposes
    // do not optimize, just acquire arbitrary lock and read bucket count
    return 0;  // to be implemented
  }

 private:
  size_t GetStripeIndex(const size_t hash_value) const {
    return hash_value % mutex_array_.size();
  }

  size_t GetBucketIndex(const size_t hash_value) const {
    return hash_value % hash_table_.size();
  }

  bool IsInBucket(const T& element, size_t bucket_index){
    for(auto iterator = hash_table_[bucket_index].begin(); iterator != hash_table_[bucket_index].end(); iterator++){
        if(*iterator == element){
            return true;
        }
    }
    return false;
  }
  
  void Resize(){
    for(size_t i = 0; i < mutex_array_.size(); ++i){
        mutex_array_[i].lock();
    }
    if (max_load_factor_ < (element_counter_.load() / hash_table_.size())){
        std::vector<std::forward_list<T>> new_hash_table(hash_table_.size() * growth_factor_);
        for(size_t i = 0; i < hash_table_.size(); ++i){
            for(auto &curr_element : hash_table_[i]){
                size_t hash_val = Hash()(curr_element);
                new_hash_table[hash_val % (new_hash_table.size())].push_front(curr_element);
            }
        }
        hash_table_.swap(new_hash_table);
    }
    for(size_t i = 0; i < mutex_array_.size(); ++i){
        mutex_array_[i].unlock();
    }
  } 
  
 private:
  size_t concurrency_level_;
  size_t growth_factor_;
  double max_load_factor_;
  std::atomic<size_t> element_counter_;
  std::vector<std::shared_timed_mutex> mutex_array_;
  std::vector<std::forward_list<T>> hash_table_;
};

template <typename T, class Hash>
bool StripedHashSet<T, Hash>::Insert(const T& element) {
    size_t hash_value = Hash()(element);
    size_t stripe_index = GetStripeIndex(hash_value);
    mutex_array_[stripe_index].lock();
    size_t bucket_index = GetBucketIndex(hash_value);
    if (IsInBucket(element, bucket_index)){
        mutex_array_[stripe_index].unlock();
        return false;
    }
    if (max_load_factor_ < (element_counter_.load() / hash_table_.size())) {
        mutex_array_[stripe_index].unlock();
        Resize();
        mutex_array_[stripe_index].lock();
    }
    bucket_index = GetBucketIndex(hash_value);
    hash_table_[bucket_index].push_front(element);
    element_counter_.fetch_add(1);
    mutex_array_[stripe_index].unlock();
    return true;
  }

template <typename T, class Hash>
bool StripedHashSet<T, Hash>::Remove(const T& element) {
    size_t hash_value = Hash()(element);
    size_t stripe_index = GetStripeIndex(hash_value);
    std::unique_lock<std::shared_timed_mutex> lock(mutex_array_[stripe_index]);
    size_t bucket_index = GetBucketIndex(hash_value);
    if(!IsInBucket(element, bucket_index)){
        return false;
    }
    hash_table_[bucket_index].remove(element);
    element_counter_.fetch_sub(1);
    return true;
  }

template <typename T, class Hash>
bool StripedHashSet<T, Hash>::Contains(const T& element) {
    size_t hash_value = Hash()(element);
    size_t stripe_index = GetStripeIndex(hash_value);
    std::shared_lock<std::shared_timed_mutex> lock(mutex_array_[stripe_index]);
    size_t bucket_index = GetBucketIndex(hash_value);
    if (IsInBucket(element, bucket_index)){
        return true;
    }
    else{
        return false;
    }
  }

template <typename T> using ConcurrentSet = StripedHashSet<T>;

}  // namespace solutions
}  // namespace tpcc
