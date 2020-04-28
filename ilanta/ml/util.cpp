#include "ilanta/ml/util.h"

namespace ilanta {

auto set_all_tensor_scales(nv::INetworkDefinition* const network, float const in_scales,
                           float const out_scales) -> void {

  /* Scale inputs */
  for (auto i = 0; i < network->getNbLayers(); ++i) {
    auto layer = network->getLayer(i);

    for (auto j = 0; i < network->getNbInputs(); ++j) {
      auto input = layer->getInput(j);

      if (input && !input->dynamicRangeIsSet())
        input->setDynamicRange(-in_scales, in_scales);
    }
  }

  /* Scale outputs */
  for (auto i = 0; i < network->getNbLayers(); ++i) {
    auto layer = network->getLayer(i);

    for (auto j = 0; j < layer->getNbOutputs(); ++j) {
      auto output = layer->getOutput(j);

      if (output && !output->dynamicRangeIsSet()) {
        if (layer->getType() == nv::LayerType::kPOOLING)
          output->setDynamicRange(-in_scales, in_scales);
        else
          output->setDynamicRange(-out_scales, out_scales);
      }
    }
  }
}

auto NvLogger::log(nv::ILogger::Severity const severity, char const* const msg) -> void {
  auto constexpr fmt_str = "TensorRT: {}";

  using namespace nv;

  switch (severity) {
  case Severity::kVERBOSE:
    spdlog::debug(fmt_str, msg);
    break;

  case Severity::kINFO:
    spdlog::info(fmt_str, msg);
    break;

  case Severity::kWARNING:
    spdlog::warn(fmt_str, msg);
    break;

  case Severity::kINTERNAL_ERROR:
    spdlog::error("TensorRT: INTERNAL: {}", msg);
    break;

  case Severity::kERROR:
    spdlog::error(fmt_str, msg);
    break;
  }
}

} // namespace ilanta
