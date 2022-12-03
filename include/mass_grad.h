#ifndef INNER_CARVING_CPP_MASS_GRAD_H
#define INNER_CARVING_CPP_MASS_GRAD_H
#include <Eigen/Core>

// Inputs:
//   V  #V by 3 vertex positions
//
// Output:
//   grad  1 by #V row vector of gradient
//
void mass_grad(
  const Eigen::MatrixXd &V,
  const Eigen::MatrixXi &F,
  Eigen::RowVectorXd &grad);
#endif //INNER_CARVING_CPP_MASS_GRAD_H
