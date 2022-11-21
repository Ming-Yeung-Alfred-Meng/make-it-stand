#ifndef CURVATURE_CENTER_OF_MASS_H
#define CURVATURE_CENTER_OF_MASS_H
#include <Eigen/Core>

// Compute the center of mass of the mesh defined
// collectively by outer mash (MoV, MoF) and inner
// mesh (MiV, MiF), according the equation 4 of the
// paper. Assumes outer and inner mesh are oriented s.t.
// normals point outward.
//
// If MiV and MiF represent an empty inner
// mesh, select the version accordingly.
//
// Inputs:
//   MoV  #V by 3 list of deformed vertex positions
//   MoF  #F by 3 list of face indices into rows of V
//   MiV  #MiV by 3 list of inner mesh vertex positions
//   MiF  #MiF by 3 list of inner mesh face indices into rows of MiV
//   density  uniform mass density of the mesh
//
// Output:
//  CoM  3D vector of the center of mass
//  m  mass of the mesh
//
void center_of_mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &MiV,
  const Eigen::MatrixXi &MiF,
  const double &density,
  Eigen::Vector3d &CoM,
  double &m);


// Version without inner mesh.
void center_of_mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const double &density,
  Eigen::Vector3d &CoM,
  double &m);
#endif
