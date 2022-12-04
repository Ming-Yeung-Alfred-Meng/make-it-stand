#ifndef INNER_CARVING_CPP_CENTER_OF_MASS_GRAD_H
#define INNER_CARVING_CPP_CENTER_OF_MASS_GRAD_H
#include <Eigen/Core>
#include <Eigen/Geometry>

// Compute the gradient of center of mass with
// respect to vertex positions.
//
// Inputs:
//   dV  #dV by 3 vertex positions
//   dF  #dF by 3 list of vertex indices into dV
//
// Output:
//   grad  3 by 3 * #dV  gradient matrix
//
void center_of_mass_grad(
  const Eigen::MatrixXd &dV,
  const Eigen::MatrixXi &dF,
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &MiV,
  const Eigen::MatrixXi &MiF,
  const double density,
  Eigen::MatrixXd &grad);
#endif //INNER_CARVING_CPP_CENTER_OF_MASS_GRAD_H
