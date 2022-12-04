#ifndef INNER_CARVING_CPP_CENTER_OF_MASS_GRAD_H
#define INNER_CARVING_CPP_CENTER_OF_MASS_GRAD_H
#include <Eigen/Core>
#include <Eigen/Geometry>

// Compute the gradient of center of mass with
// respect to vertex positions.
//
// Inputs:
//   V  #V by 3 vertex positions
//   F  #F by 3 list of vertex indices into V
//
// Output:
//   grad  3 by 3 * #V  gradient matrix
//
void center_of_mass_grad(
  const Eigen::MatrixXd &V,
  const Eigen::MatrixXi &F,
  const double density,
  Eigen::MatrixXd &grad);
#endif //INNER_CARVING_CPP_CENTER_OF_MASS_GRAD_H
