#include "ilanta/ml/buffer_manager.h"
#include <NvInferRuntime.h>
#include <memory>

namespace ilanta {

BufferManager::BufferManager(nv::IExecutionContext const* const ctx,
                             std::shared_ptr<nv::ICudaEngine> engine, int const batch_size) {}

} // namespace ilanta
