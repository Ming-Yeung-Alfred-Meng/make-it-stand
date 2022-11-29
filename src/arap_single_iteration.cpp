#include "arap_single_iteration.h"
#include <igl/min_quad_with_fixed.h>
#include <igl/polar_svd3x3.h>


void local_step(
  const Eigen::SparseMatrix<double> & K,
  const Eigen::MatrixXd & U,
  Eigen::MatrixXd & Rotation)
{
  Eigen::Matrix3d R;
  Rotation = Eigen::MatrixXd(3 * U.rows(), 3);
  Eigen::MatrixXd C = K.transpose() * U;

  for (int i = 0; i < C.rows(); i += 3) {
    igl::polar_svd3x3(Eigen::Matrix3d(C.block<3, 3>(i, 0)), R);
    Rotation.block<3, 3>(i, 0) = R;
  }
}


void global_step(
  const igl::min_quad_with_fixed_data<double> & data,
  const Eigen::MatrixXd & Rotation,
  const Eigen::SparseMatrix<double> & K,
  const Eigen::MatrixXd & bc,
  Eigen::MatrixXd & U
)
{
  igl::min_quad_with_fixed_solve(data, - K * Rotation / 6, bc, Eigen::Matrix<double, 0, 0>(), U);
}


void arap_single_iteration(
  const igl::min_quad_with_fixed_data<double> & data,
  const Eigen::SparseMatrix<double> & K,
  const Eigen::MatrixXd & bc,
  Eigen::MatrixXd & U)
{
  Eigen::MatrixXd Rotation;
  local_step(K, U, Rotation);
  global_step(data, Rotation, K, bc, U);
}
