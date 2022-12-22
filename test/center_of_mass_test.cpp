#include "gtest/gtest.h"
#include "igl/centroid.h"
#include "igl/readOBJ.h"
#include "center_of_mass.h"


class CenterOfMassOuterMeshOnlyTest : public ::testing::Test {
protected:
    void SetUp(const std::string & filename, const double d) {
      igl::readOBJ(filename, V, F);
      density = d;
      center_of_mass(V, F, density, CoM, mass);
      igl::centroid(V, F, centroid, volume);
    }

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    Eigen::Vector3d CoM;
    Eigen::Vector3d centroid;
    double mass {};
    double volume {};
    double density {};
};


class CenterOfMassOuterInnerMeshTest : public ::testing::Test {
protected:
    void SetUp(const std::string & filename, const double d) {
      igl::readOBJ(filename, MoV, MiF);
      density = d;
      center_of_mass(MoV, MoF, density, CoM, mass);
      igl::centroid(MoV, MoF, centroid, volume);
    }

    Eigen::MatrixXd MoV;
    Eigen::MatrixXi MoF;
    Eigen::MatrixXd MiV;
    Eigen::MatrixXi MiF;
    Eigen::MatrixXd V_combined;
    Eigen::MatrixXi F_combined;
    Eigen::Vector3d CoM;
    Eigen::Vector3d centroid;
    double mass {};
    double volume {};
    double density {};
};

// Center of mass of a cube with min max corner (-20, -20, 0) and (0, 0, 20), respectively.
TEST_F(CenterOfMassOuterMeshOnlyTest, Cube) {
  SetUp("../../data/cube.obj", 3);

  EXPECT_TRUE(CoM.isApprox(centroid));
  EXPECT_NEAR(mass, density * volume, 1e-10);
}


TEST_F(CenterOfMassOuterMeshOnlyTest, Sphere) {
  SetUp("../../data/sphere.obj", 3);

  EXPECT_TRUE(CoM.isApprox(centroid));
  EXPECT_NEAR(mass, density * volume, 1e-10);
}


TEST_F(CenterOfMassOuterMeshOnlyTest, Cylinder) {
  SetUp("../../data/cylinder.obj", 3);

  EXPECT_TRUE(CoM.isApprox(centroid));
  EXPECT_NEAR(mass, density * volume, 1e-10);
}
