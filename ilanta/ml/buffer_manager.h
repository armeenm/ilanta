#pragma once

#include <NvInfer.h>
#include <NvInferRuntime.h>
#include <memory>

namespace ilanta {

namespace nv = nvinfer1;

class BufferManager {
public:
  auto constexpr static invalid_size_value = ~size_t{0};

  [[nodiscard]] BufferManager(std::shared_ptr<nv::ICudaEngine> engine, int batch_size,
                              nv::IExecutionContext const* context);

  BufferManager(BufferManager const&) = delete;
  BufferManager(BufferManager&&) noexcept = default;

  auto operator=(BufferManager const&) -> BufferManager& = delete;
  auto operator=(BufferManager&&) noexcept -> BufferManager& = default;

  ~BufferManager() = default;
};

} // namespace ilanta
