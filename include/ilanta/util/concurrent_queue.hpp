#pragma once

#include <queue>

namespace ilanta {

template <typename T>
class ConcurrentQueue {
 public:
 private:
  std::queue<T> data_;
};

}  // namespace ilanta
