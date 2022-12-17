#include "gtest/gtest.h"
#include "helper_g.h"

TEST(ExampleTest, ExampleTestName) {
  Eigen::Vector3d g = Eigen::Vector3d::Zero();
  helper_g(Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero(), g);
  EXPECT_EQ(g, Eigen::Vector3d::Ones());
}