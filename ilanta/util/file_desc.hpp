#pragma once

extern "C" {
#include <fcntl.h>
}

namespace ilanta {

class FileDesc {
  FileDesc(std::string_view path, int flags);

  FileDesc(FileDesc const&);
  FileDesc(FileDesc&&);

  auto operator=(FileDesc const&) -> FileDesc&;
  auto operator=(FileDesc &&) -> FileDesc&;
};

}; // namespace ilanta
