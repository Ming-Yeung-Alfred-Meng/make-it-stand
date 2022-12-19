#include "gtest/gtest.h"
#include "igl/centroid.h"
#include "igl/readOBJ.h"
#include "center_of_mass.h"


// Center of mass of a cube with min max corner (-20, -20, 0) and (0, 0, 20), respectively.
TEST(ExampleTest, ExampleTestName) {
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  igl::readOBJ("../../data/cube.obj", V, F);

  assert(V.cols() == 3);
  assert(F.cols() == 3);

  Eigen::Vector3d CoM;
  Eigen::Vector3d centroid;
  double mass;
  double volume;
  const double density = 3;

  center_of_mass(V, F, density, CoM, mass);
  igl::centroid(V, F, centroid, volume);

  EXPECT_TRUE(CoM.isApprox(centroid));
  EXPECT_DOUBLE_EQ(mass, density * volume);
}
