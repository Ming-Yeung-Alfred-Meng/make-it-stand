#ifndef MAKE_IT_STAND_VOXEL_INDICES_H
#define MAKE_IT_STAND_VOXEL_INDICES_H
#include <Eigen/Core>

// Build vector of indices of voxel centers within the mesh.
//
// Inputs:
//   MoV #V by 3 list of deformed vertex positions
//   MoF #F by 3 list of face indices into rows of V
//   grid  #grid by 3 matrix of voxel grid centers
//   min  minimum distance from voxel center to mesh surface
//
// Output:
//  indices (into grid) of voxel centers within the mesh.
void voxel_indices(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &grid,
  const double &min,
  std::vector<int> &indices);

#endif //MAKE_IT_STAND_VOXEL_INDICES_H
