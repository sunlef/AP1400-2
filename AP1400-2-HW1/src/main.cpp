
#include "hw1.h"
#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char **argv) {
  if (false) // make false to run unit-tests
  {
    Matrix tmp = algebra::random(5, 4, 15160825, 13918679613);
    algebra::show(tmp);
    // debug section
  } else {
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "RUNNING TESTS ..." << std::endl;
    int ret{RUN_ALL_TESTS()};
    if (!ret)
      std::cout << "<<<SUCCESS>>>" << std::endl;
    else
      std::cout << "FAILED" << std::endl;
  }
  return 0;
}