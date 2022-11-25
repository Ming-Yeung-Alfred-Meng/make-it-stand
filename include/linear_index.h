#ifndef MAKE_IT_STAND_LINEAR_INDEX_H
#define MAKE_IT_STAND_LINEAR_INDEX_H
#include <Eigen/Core>


// Return the linear index of an element in a 3D
// array-like structure.
//
// Inputs:
//   side  3 row vector s.t. side(i) is the length of the ith side.
//   i  ith coordinate
//   j  jth coordinate
//   k  kth coordinate
//
// Output:
//   n  linear index
Eigen::Index linear_index(
  const Eigen::RowVector3i &side,
  const int i,
  const int j,
  const int k);
#endif //MAKE_IT_STAND_LINEAR_INDEX_H
