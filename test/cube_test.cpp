#include "gtest/gtest.h"
#include <igl/opengl/glfw/Viewer.h>
#include <igl/readOBJ.h>




TEST(name, name) {
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;

  igl::opengl::glfw::Viewer viewer;

  ASSERT_TRUE(igl::readOBJ("../../data/cube.obj", V, F));
  viewer.data().set_mesh(V, F);
  viewer.launch();
}