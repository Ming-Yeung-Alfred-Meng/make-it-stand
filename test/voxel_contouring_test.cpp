#include <algorithm>
#include <gtest/gtest.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/voxel_grid.h>
#include <voxel_contouring.h>

// construct a voxel grid
// load the voxels
// call voxel_contouring

class VoxelContouringTest : public ::testing::Test {
protected:
    void SetUp(int in_out[]) {
      igl::voxel_grid(Eigen::AlignedBox<double, 3>(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(3, 3, 3)), 4, 0, grid, side);
      voxel_contouring(grid, side, in_out, V, F);
      viewer.data().set_mesh(V, F);
    }

    Eigen::MatrixXd grid;
    Eigen::RowVector3i side;
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    igl::opengl::glfw::Viewer viewer;
};


TEST_F(VoxelContouringTest, CenterCube) {
  int corner[64];
  std::fill(std::begin(corner), std::end(corner), 1);
  corner[21] = -1;
  corner[22] = -1;
  corner[25] = -1;
  corner[26] = -1;
  corner[37] = -1;
  corner[38] = -1;
  corner[41] = -1;
  corner[42] = -1;

  SetUp(corner);

  Eigen::Vector3d min_corner = V.colwise().minCoeff();
  Eigen::Vector3d max_corner = V.colwise().maxCoeff();

  Eigen::Vector3d diagonal = max_corner - min_corner;
  Eigen::Vector3d proj_diagonal = diagonal;
  proj_diagonal(2) = 0;

  ASSERT_TRUE((min_corner + 0.5 * diagonal).isApprox(Eigen::Vector3d(1.5, 1.5, 1.5)));
  ASSERT_NEAR(diagonal.norm(), sqrt(3) * 2, 1e-10);
  ASSERT_NEAR(acos(diagonal.dot(proj_diagonal) / (diagonal.norm() * proj_diagonal.norm())), atan2(1, sqrt(2)), 1e-10);

  viewer.launch();
}


TEST_F(VoxelContouringTest, Irregular) {
  int corner[64];
  std::fill(std::begin(corner), std::end(corner), 1);
  corner[21] = -1;
  corner[25] = -1;
  corner[26] = -1;
  corner[41] = -1;

  SetUp(corner);
  viewer.launch();
}
