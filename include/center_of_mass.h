#ifndef CURVATURE_CENTER_OF_MASS_H
#define CURVATURE_CENTER_OF_MASS_H
#include <Eigen/Core>

// Compute the center of mass of the mesh defined by
// outer mash (MoV, MoF) and inner mesh (MiV, MiF).
//
// Input:
//   MoV #V by 3 list of deformed vertex positions
//   MoF #F by 3 list of face indices into rows of V
//   MiV #MiV by 3 list of inner mesh vertex positions
//   MiF #MiF by 3 list of inner mesh face indices into rows of MiV
//   p uniform mass density of the mesh
//
// Output:
//  C 3D vector of the center of mass
//
void center_of_mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &MiV,
  const Eigen::MatrixXi &MiF,
  const double p,
  Eigen::Vector3d &C);
#endif
