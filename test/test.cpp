#define CATCH_CONFIG_MAIN

#include "ilanta/control/pose.hpp"
#include "ilanta/math/util.hpp"
#include <catch2/catch.hpp>
#include <cmath>

using namespace ilanta;

TEST_CASE("Mathematical operations can be performed on PoseTL objects", "[PoseTL]") {
  auto constexpr p1 = PoseTL{0.0f, 3.5f, 2.2f};
  auto constexpr p2 = PoseTL{1.1f, 5.6f, 10.1f};

  REQUIRE(p1 + p2 == PoseTL{p1.x() + p2.x(), p1.y() + p2.y(), p1.z() + p2.z()});
  REQUIRE(p1 - p2 == PoseTL{p1.x() - p2.x(), p1.y() - p2.y(), p1.z() - p2.z()});
  REQUIRE(approx_equal(p1.l2norm(), std::sqrt(p1.x() * p1.x() + p1.y() * p1.y() + p1.z() * p1.z()),
                       0.2f));
}
