#ifndef CURVATURE_MASS_H
#define CURVATURE_MASS_H
#include <Eigen/Core>
#include <Eigen/Geometry>

// Compute the mass of the mesh defined by
// outer mash (MoV, MoF) and inner mesh (MiV, MiF).
//
// If MiV and MiF represent an empty inner
// mesh, select the version accordingly.
//
// Inputs:
//   MoV #V by 3 list of deformed vertex positions
//   MoF #F by 3 list of face indices into rows of V
//   MiV #MiV by 3 list of inner mesh vertex positions
//   MiF #MiF by 3 list of inner mesh face indices into rows of MiV
//   density uniform mass density of the mesh
//
// Return:
//  mass
//
double mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &MiV,
  const Eigen::MatrixXi &MiF,
  const double density);


// Version without inner mesh.
double mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const double density);
#endif //CURVATURE_MASS_H
