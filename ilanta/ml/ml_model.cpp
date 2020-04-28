#include "ilanta/ml/ml_model.hpp"
#include "ilanta/ml/generic_buffer.h"
#include "ilanta/ml/util.h"

#include <NvInfer.h>
#include <NvOnnxParser.h>
#include <cassert>
#include <cstdint>
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace ilanta {

MLModel::MLModel(std::string_view onnx_filename, MLModel::TensorNames tensor_names,
                 MLModel::Options const& opts)
    : filename_{onnx_filename}, tensor_names_{tensor_names}, opts_{opts} {
  auto builder = InferUniquePtr<nv::IBuilder>{nv::createInferBuilder(nv_logger)};
  if (!builder)
    throw std::runtime_error("Failed to create builder");

  auto const explicit_batch =
      1U << static_cast<std::uint32_t>(nv::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);
  auto network = InferUniquePtr<nv::INetworkDefinition>{builder->createNetworkV2(explicit_batch)};
  if (!network)
    throw std::runtime_error("Failed to create network");

  auto config = InferUniquePtr<nv::IBuilderConfig>{builder->createBuilderConfig()};
  if (!config)
    throw std::runtime_error("Failed to create builder config");

  auto parser =
      InferUniquePtr<nvonnxparser::IParser>{nvonnxparser::createParser(*network, nv_logger)};
  if (!parser)
    throw std::runtime_error("Failed to create parser");

  /* Build Network */

  auto parsed = parser->parseFromFile(filename_.c_str(), static_cast<int>(opts_.log_level));
  if (!parsed)
    throw std::runtime_error("Failed to parse file");

  // TODO: Make these things user-specified...
  builder->setMaxBatchSize(opts_.batch_size);
  config->setMaxWorkspaceSize(16 * (1 << 20)); // 16 MiB

  if (opts_.enable_fp16)
    config->setFlag(nv::BuilderFlag::kFP16);

  if (opts_.enable_int8) {
    config->setFlag(nv::BuilderFlag::kINT8);
    // TODO: Set all tensor scales
  }

  /* Create Engine */

  engine_ = {builder->buildEngineWithConfig(*network, *config), InferDeleter()};
  if (!engine_)
    throw std::runtime_error("Failed to create engine");

  assert(network->getNbInputs() == 1);
  input_dims_ = network->getInput(0)->getDimensions();

  // assert(network->getNbOutputs() == 1);
  output_dims_ = network->getOutput(0)->getDimensions();
}

auto MLModel::filename() const noexcept -> std::string_view { return filename_; }
auto MLModel::opts() const noexcept -> MLModel::Options { return opts_; }
auto MLModel::engine() const noexcept -> std::shared_ptr<nv::ICudaEngine> { return engine_; }
auto MLModel::input_dims() const noexcept -> nv::Dims { return input_dims_; }
auto MLModel::output_dims() const noexcept -> nv::Dims { return output_dims_; }

auto MLModel::infer() -> bool {
  auto context = InferUniquePtr<nv::IExecutionContext>{engine_->createExecutionContext()};
  if (!context)
    return false;

  assert(tensor_names_.input.size() == 1);

  return true;
}

} // namespace ilanta
