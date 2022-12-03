#include "my_lbs_matrix.h"

void my_lbs_matrix(
  const Eigen::VectorXd &V,
  const Eigen::MatrixXd &W,
  Eigen::MatrixXd &M)
{
  assert(W.rows() == V.rows() && V.rows() % 3 == 0);
  M.resize(V.rows(), 4 * W.cols());
  M.setZero();

  for (int i = 0; i < M.rows(); ++i) {
    for (int j = 0; j < W.cols(); ++j) {
      M(i, 4 * j + (i % 3)) = W(i / 3, j);
      M(i, 4 * j + 3) = W(i / 3, j) * V(i);
    }
  }
}