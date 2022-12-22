#include <gtest/gtest.h>
#include <igl/readOBJ.h>
#include <voxel_indices.h>

class VoxelIndicesTest : public ::testing::Test {
protected:
    void SetUp() override {
      igl::readOBJ("../../data/sphere2.obj", V, F);
      // A sphere of radius 10, centered at the origin.
    }

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    const double min = 5;
};

TEST_F(VoxelIndicesTest, InsideMesh) {
  Eigen::MatrixXd grid(1, 3);
  grid << 1, 1, 1;

  std::vector<int> indices;
  voxel_indices(V, F, grid, min, indices);

  EXPECT_TRUE(indices[0] == 0);
}


TEST_F(VoxelIndicesTest, TooCloseToHull) {
  Eigen::MatrixXd grid(1, 3);
  grid << 5, 5, 5;

  std::vector<int> indices;
  voxel_indices(V, F, grid, min, indices);

  EXPECT_TRUE(indices.empty());
}


TEST_F(VoxelIndicesTest, OutsideMesh) {
  Eigen::MatrixXd grid(1, 3);
  grid << 10, 10, 10;

  std::vector<int> indices;
  voxel_indices(V, F, grid, min, indices);

  EXPECT_TRUE(indices.empty());
}