#include "my_lbs_matrix.h"
#include <igl/bbw.h>
#include "update_lbs_matrix.h"

void my_lbs_matrix(
  const Eigen::VectorXd &V,
  const Eigen::MatrixXd &W,
  Eigen::MatrixXd &M)
{
  // compute bbw
  // initialize a matrix of all 0s
  // fill in 1s in the appropriate places

    // go through each row, if that row id by mod 3, if 0, insert 1 at column 4 * j
    // 1, insert at column 4 * j + 1, 2 then 4 * j + 2

  // apply the weights
  // use update lbs_matrix to fill in the vertex positions
  assert(W.rows() == V.rows() && V.rows() % 3 == 0);
  M.resize(V.rows(), 4 * W.cols());
  M.setZero();

  for (int i = 0; i < M.rows(); ++i) {
    for (int j = 0; j < W.cols(); ++j) {
      M(i, 4 * j + (i % 3)) = W(i / 3, j);
    }
  }

  update_lbs_matrix(V, W, M);
}