#include <gtest/gtest.h>

#include "cfg/api/cfg.hpp"

int main(int argc, char **argv)
{
    Simulator::Cfg::init();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}