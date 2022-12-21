#include <gtest/gtest.h>
#include <helper_g.h>


TEST(HelperGTest, ArbitraryVectors) {
  Eigen::Vector3d g;
  helper_g(Eigen::Vector3d(7.34, 9.719, 7.997),
           Eigen::Vector3d(7.818, 7.739, 2.619),
           Eigen::Vector3d(7.394, 1.233, 1.033),
           g);

  EXPECT_TRUE(g.isApprox(Eigen::Vector3d(339.130332, 252.612426, 103.78873)));
}