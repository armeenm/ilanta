#pragma once

#include <NvInfer.h>
#include <type_traits>

namespace ilanta {

namespace nv = nvinfer1;

template <typename Alloc> class GenericBuffer {
public:
  [[nodiscard]] GenericBuffer(nv::DataType dtype);

  GenericBuffer(GenericBuffer const&) = delete;
  GenericBuffer(GenericBuffer&&) noexcept = default;

  auto operator=(GenericBuffer const&) -> GenericBuffer& = delete;
  auto operator=(GenericBuffer&&) noexcept -> GenericBuffer& = default;

  ~GenericBuffer() = default;
};

} // namespace ilanta
