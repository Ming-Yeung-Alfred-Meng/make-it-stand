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
  Eigen::RowVectorXd S;
  Eigen::RowVectorXd I;
  Eigen::MatrixXd C;
  Eigen::MatrixXd N;

  for (int i = 0; i < grid.rows(); ++i) {
    igl::signed_distance(grid.row(i), MoV, MoF, igl::SIGNED_DISTANCE_TYPE_FAST_WINDING_NUMBER, S, I, C, N);

    if (S(0) <= 0 && std::abs(S(0)) >= min) {
      indices.emplace_back(i);
    }
  }

  // go through each point in grid
  // call signed_distance
  // if sign is correct/within hull and abs distance is greater than min, put its index into indices vector
}