#pragma once

#include <NvInfer.h>
#include <memory>
#include <spdlog/spdlog.h>

namespace ilanta {

namespace nv = nvinfer1;

auto set_all_tensor_scales(nv::INetworkDefinition* network, float in_scales, float out_scales)
    -> void;

class NvLogger : public nv::ILogger {
  void log(nv::ILogger::Severity severity, char const* msg) override;
};

struct InferDeleter {
  template <typename T> auto operator()(T* const obj) const -> void {
    if (obj)
      obj->destroy();
  }
};

template <typename T> using InferUniquePtr = std::unique_ptr<T, InferDeleter>;

inline auto nv_logger = NvLogger{};

} // namespace ilanta
