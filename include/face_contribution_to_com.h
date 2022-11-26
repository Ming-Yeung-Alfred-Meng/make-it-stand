#ifndef MAKE_IT_STAND_FACE_CONTRIBUTION_TO_COM_H
#define MAKE_IT_STAND_FACE_CONTRIBUTION_TO_COM_H
#include <Eigen/Core>

// Compute the contribution of faces of a (outer or inner) mesh
// to the center of mass of the mesh defined collectively by the
// outer and inner mesh.
//
// Inputs:
//   V  #V by 3 list of vertex positions
//   F  #F by 3 list of face indices into rows of V
//
// Output:
//   con contribution
//
void face_contribution_to_com(
  const Eigen::MatrixXd &V,
  const Eigen::MatrixXi &F,
  Eigen::Vector3d &con);
#endif //MAKE_IT_STAND_FACE_CONTRIBUTION_TO_COM_H
