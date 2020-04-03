#define CATCH_CONFIG_MAIN

#include "ilanta/control/pose.hpp"
#include "ilanta/math/util.hpp"
#include <catch2/catch.hpp>
#include <cmath>

using namespace ilanta;
using namespace Catch::literals;

TEST_CASE("Mathematical operations can be performed on PoseTL objects", "[PoseTL]") {
  auto constexpr p1 = PoseTL{0.0f, 3.5f, 2.2f};
  auto constexpr p2 = PoseTL{1.1f, 5.6f, 10.1f};

  REQUIRE(p1 + p2 == PoseTL{0.0f + 1.1f, 3.5f + 5.6f, 2.2f + 10.1f});
  REQUIRE(p1 + PoseTL{1.0f, 2.0f, 3.0f} < p2);
  REQUIRE(p1.l2norm() == 4.13401_a);
}
