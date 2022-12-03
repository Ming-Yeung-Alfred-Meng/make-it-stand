#ifndef INNER_CARVING_CPP_CENTER_OF_MASS_GRAD_H
#define INNER_CARVING_CPP_CENTER_OF_MASS_GRAD_H
#include <Eigen/Core>

void center_of_mass_grad(
  const Eigen::MatrixXd &V,
  const Eigen::MatrixXi &F,
  Eigen::MatrixXd &grad);
#endif //INNER_CARVING_CPP_CENTER_OF_MASS_GRAD_H
