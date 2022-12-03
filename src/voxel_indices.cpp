#include <cmath>
#include <igl/signed_distance.h>
#include "voxel_indices.h"


void voxel_indices(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &grid,
  const double &min,
  std::vector<int> &indices)
{
  Eigen::VectorXd S;
  Eigen::VectorXd I;
  Eigen::MatrixXd C;
  Eigen::MatrixXd N;

  igl::signed_distance(grid, MoV, MoF, igl::SIGNED_DISTANCE_TYPE_FAST_WINDING_NUMBER, S, I, C, N);

  for (int i = 0; i < grid.rows(); ++i) {
    if (S(i) <= 0 && std::abs(S(i)) >= min) {
      indices.emplace_back(i);
    }
  }

  std::cerr << "S:\n" << S << std::endl;
  std::cerr << "indices:\n" << std::endl;
  for (auto i : indices) {
    std::cerr << i << std::endl;
  }

}