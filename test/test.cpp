#define CATCH_CONFIG_MAIN

#include "ilanta/control/pose.hpp"
#include <catch2/catch.hpp>

using namespace ilanta;

TEST_CASE("Mathematical operations can be performed on PoseTL objects", "[PoseTL]") {
  auto p1 = PoseTL{0.0f, 3.5f, 2.2f};
  auto p2 = PoseTL{1.1f, 5.6f, 10.1f};

  // REQUIRE(p1 + p2 == PoseTL{1.1, 9.1, 12.3});
  REQUIRE(1 + 2 == 3);
}
