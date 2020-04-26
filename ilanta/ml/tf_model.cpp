#include "ilanta/ml/tf_model.h"

#include <fmt/core.h>
#include <stdexcept>
#include <tensorflow/cc/saved_model/tag_constants.h>
#include <tensorflow/core/lib/core/status.h>

namespace tf = tensorflow;

TFModel::TFModel(std::string_view model_dir) {
  auto status = tf::LoadSavedModel(tf::SessionOptions(), tf::RunOptions(), model_dir.data(),
                                   {tf::kSavedModelTagServe}, &model_);
  if (!status.ok())
    throw std::runtime_error(status.ToString());

  sig_def_ = model_.GetSignatures().at(tf::kDefaultServingSignatureDefKey);

  auto input_name = sig_def_.inputs().at("input").name();
  input_vec_ = {{std::move(input_name), tf::Tensor()}};

  output_name_ = sig_def_.outputs().at("output").name();
}

auto TFModel::predict(tf::Tensor input_tensor) -> tf::Tensor {
  using namespace std::string_literals;

  input_vec_[0].second = input_tensor;

  auto status = model_.GetSession()->Run(input_vec_, {output_name_}, {}, &output_vec_);
  if (!status.ok())
    throw std::runtime_error("Inference failed: "s + status.ToString());

  return output_vec_[0];
}
