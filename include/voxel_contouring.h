#ifndef MAKE_IT_STAND_VOXEL_CONTOURING_H
#define MAKE_IT_STAND_VOXEL_CONTOURING_H
#include <Eigen/Core>

// Build a voxel-like mesh defined inside a voxel grid.
//
// Inputs:
//   grid  #grid by 3 voxel centers of a regular voxel grid
//   side  3 row vector, where side[i] is the number of cells of the ith side
//   in_out  in_out[i] = 1 if the ith cell in grid is outside the voxel-like mesh,
//           in_out[i] = -1 if the it is inside.
//
// Outputs:
//   V  #V by 3 list of vertex positions
//   F  #F by 3 list of oriented face indices into rows of V
//
void voxel_contouring(
  const Eigen::MatrixXd &grid,
  const Eigen::RowVector3i &side,
  const int in_out[],
  Eigen::MatrixXd &V,
  Eigen::MatrixXi &F);
#endif //MAKE_IT_STAND_VOXEL_CONTOURING_H
