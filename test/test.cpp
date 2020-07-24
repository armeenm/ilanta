#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <fmt/format.h>
#include <gmock/gmock.h>

#include <ilanta/util/concurrent_queue.hpp>
#include <thread>

TEST_CASE("Concurrent queues are thread-safe") {
  auto consumer = std::jthread{[] { fmt::print("Test!\n"); }};
}
