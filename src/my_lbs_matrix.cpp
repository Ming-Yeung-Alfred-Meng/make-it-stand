#include "my_lbs_matrix.h"
#include <igl/bbw.h>

void my_lbs_matrix(
  const Eigen::VectorXd &V,
  const Eigen::MatrixXd &W,
  Eigen::MatrixXd &M)
{
  // compute bbw
  // initialize a matrix of all 0s
  // fill in 1s in the appropriate places
  // apply the weights
  // use update lbs_matrix to fill in the vertex positions

  M.resize(V.rows(), 4 * W.cols());
  M.setZero();


//  update_lbs_matrix(V, W, M);
}