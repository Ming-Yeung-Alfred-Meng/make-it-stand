#include <random>
#include <cmath>
#include "gtest/gtest.h"
#include <igl/opengl/glfw/Viewer.h>
#include <igl/readOBJ.h>
#include <cube.h>


TEST(CubeTest, RandomVoxel) {
  const Eigen::Vector3d center = Eigen::Vector3d::Random() * 10;

  std::uniform_real_distribution<double> unif(0.00001,1);
  std::default_random_engine re;
  const double length = unif(re);

  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  cube(center, length, V, F);

  Eigen::Vector3d min_corner = V.colwise().minCoeff();
  Eigen::Vector3d max_corner = V.colwise().maxCoeff();

  Eigen::Vector3d diagonal = max_corner - min_corner;
  Eigen::Vector3d proj_diagonal = diagonal;
  proj_diagonal(2) = 0;

  ASSERT_TRUE((min_corner + 0.5 * diagonal).isApprox(center));
  ASSERT_NEAR(diagonal.norm(), sqrt(3) * length, 1e-10);
  ASSERT_NEAR(acos(diagonal.dot(proj_diagonal) / (diagonal.norm() * proj_diagonal.norm())), atan2(1, sqrt(2)), 1e-10);

  igl::opengl::glfw::Viewer viewer;
  viewer.data().set_mesh(V, F);
  viewer.launch();
}