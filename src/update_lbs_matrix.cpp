#include "update_lbs_matrix.h"

void update_lbs_matrix(
  const Eigen::VectorXd &V,
  const Eigen::MatrixXd &W,
  Eigen::MatrixXd &M)
{
  assert(W.rows() == V.rows() && V.rows() % 3 == 0);
  assert(M.rows() == V.rows());
  assert(M.cols() == 4 * W.cols());

  for (int i = 0; i < M.rows(); ++i) {
    for (int j = 0; j < W.cols(); ++j) {
      M(i, 4 * j + 3) = W(i / 3, j) * V(i);
    }
  }
}