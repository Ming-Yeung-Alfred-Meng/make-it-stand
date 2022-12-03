#ifndef INNER_CARVING_CPP_LAPLACIAN_H
#define INNER_CARVING_CPP_LAPLACIAN_H
#include <Eigen/Sparse>

// **negative** semi-definite laplacian L
// suitable for L * V, where V is a #V vector
// of vertically stacked vertex positions.
//
// Inputs:
//   V  #V by 3 vertex positions
//   F  #F by 3 list of element indices
//
// Output:
//   Lap  3 * #V by 3 * #V Laplacian matrix
//
void laplacian(
  const Eigen::MatrixXd &V,
  const Eigen::MatrixXi &F,
  Eigen::SparseMatrix<double> &Lap);
#endif //INNER_CARVING_CPP_LAPLACIAN_H
