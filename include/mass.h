#ifndef CURVATURE_MASS_H
#define CURVATURE_MASS_H
#include <Eigen/Core>

// Compute the mass of the mesh defined by
// outer mash (MoV, MoF) and inner mesh (MiV, MiF).
//
// Inputs:
//   MoV #V by 3 list of deformed vertex positions
//   MoF #F by 3 list of face indices into rows of V
//   MiV #MiV by 3 list of inner mesh vertex positions
//   MiF #MiF by 3 list of inner mesh face indices into rows of MiV
//   p uniform mass density of the mesh
//
// Return:
//  mass
//
double mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &MiV,
  const Eigen::MatrixXi &MiF,
  const double p);
#endif //CURVATURE_MASS_H
