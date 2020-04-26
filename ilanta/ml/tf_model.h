#pragma once

#include <tensorflow/cc/saved_model/loader.h>
#include <tensorflow/cc/saved_model/signature_constants.h>

#include <cstdint>
#include <string_view>
#include <utility>
#include <vector>

namespace tf = tensorflow;

class TFModel {
public:
  TFModel(std::string_view model_dir);

  TFModel(TFModel const&) = default;
  TFModel(TFModel&&) noexcept = default;

  auto operator=(TFModel const&) -> TFModel& = default;
  auto operator=(TFModel&&) noexcept -> TFModel& = default;

  ~TFModel() = default;

  [[nodiscard]] auto predict(tf::Tensor input_tensor) -> tf::Tensor;

private:
  tf::SavedModelBundleLite model_;
  tf::SignatureDef sig_def_;

  std::vector<std::pair<tf::string, tf::Tensor>> input_vec_;

  tf::string output_name_;
  std::vector<tf::Tensor> output_vec_;
};
