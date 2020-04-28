#pragma once

#include <NvInfer.h>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace ilanta {

namespace nv = nvinfer1;

class MLModel {
public:
  struct Options {
    std::vector<std::string> input_names, output_names;
    bool enable_fp16 = false;
    bool enable_int8 = false;
    int batch_size = -1;
    nv::ILogger::Severity log_level = nv::ILogger::Severity::kWARNING;
  };

  struct TensorNames {
    std::span<std::string> input, output;
  };

  [[nodiscard]] MLModel(std::string_view onnx_filename, TensorNames, Options const&);

  MLModel(MLModel const&) = delete;
  MLModel(MLModel&&) noexcept = default;

  auto operator=(MLModel const&) -> MLModel& = delete;
  auto operator=(MLModel&&) noexcept -> MLModel& = default;

  ~MLModel() = default;

  [[nodiscard]] auto filename() const noexcept -> std::string_view;
  [[nodiscard]] auto opts() const noexcept -> Options;
  [[nodiscard]] auto engine() const noexcept -> std::shared_ptr<nv::ICudaEngine>;
  [[nodiscard]] auto input_dims() const noexcept -> nv::Dims;
  [[nodiscard]] auto output_dims() const noexcept -> nv::Dims;

  [[nodiscard]] auto infer() -> bool;

private:
  std::string filename_;
  TensorNames tensor_names_;
  Options opts_;
  std::shared_ptr<nv::ICudaEngine> engine_;
  nv::Dims input_dims_, output_dims_;
};

} // namespace ilanta
