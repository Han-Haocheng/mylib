//
// Created by HanHaocheng on 2024/6/4.
//


#include "include.h"

int main(int argc, char** argv) {
  ::testing::GTEST_FLAG(catch_exceptions) = false;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}