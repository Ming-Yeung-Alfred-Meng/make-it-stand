#include "biharmonic_solve.h"
#include <igl/min_quad_with_fixed.h>

void biharmonic_solve(
  const igl::min_quad_with_fixed_data<double> & data,
  const Eigen::MatrixXd & bc,
  Eigen::MatrixXd & D)
{
  // I wonder what happen if I substitute the zero matrices with null or uninitialized variables;
  // igl::min_quad_with_fixed_solve(data, Eigen::MatrixXd::Zero(data.n,3), bc, Eigen::MatrixXd::Zero(data.n,3), D);
  
  igl::min_quad_with_fixed_solve(data, Eigen::MatrixXd::Zero(data.n,1), bc, Eigen::Matrix<double, 0, 0>(), D);
}
