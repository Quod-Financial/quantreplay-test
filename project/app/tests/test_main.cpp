#include <gtest/gtest.h>

#include "cfg/api/cfg.hpp"

auto main(int argc, char **argv) -> int {
  testing::InitGoogleTest(&argc, argv);

  Simulator::Cfg::init();

  return RUN_ALL_TESTS();
}