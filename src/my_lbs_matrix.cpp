#include "my_lbs_matrix.h"

void my_lbs_matrix(
  const Eigen::VectorXd &V,
  const Eigen::MatrixXd &W,
  Eigen::MatrixXd &M)
{
  assert(W.rows() == V.rows());
  M.resize(3 * V.rows(), 4 * W.cols());
  M.setZero();

  int top_left_i;
  int top_left_j;

  for (int i = 0; i < V.rows(); ++i) {
    for (int j = 0; j < W.cols(); ++j) {
      top_left_i = 3 * i;
      top_left_j = 4 * j;
      M.block(top_left_i, top_left_j, 3, 3).setIdentity();
      M.block(top_left_i, top_left_j + 3, 3, 1) = V.row(i).transpose();
      M.block(top_left_i, top_left_j, 3, 4) *= W(i, j);
    }
  }
}