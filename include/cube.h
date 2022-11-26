#ifndef MAKE_IT_STAND_CUBE_H
#define MAKE_IT_STAND_CUBE_H
#include <Eigen/Core>

// Build the mesh of a single regular voxel.
//
// Inputs:
//   center  3 vector of voxel's position
//   length  length of each side of the regular voxel.
//
// Outputs:
//   V  #V by 3 list of vertex positions
//   F  #F by 3 list of oriented face indices into rows of V
//
void cube(
  const Eigen::Vector3d &center,
  const double length,
  Eigen::MatrixXd &V,
  Eigen::MatrixXi &F);
#endif //MAKE_IT_STAND_CUBE_H
