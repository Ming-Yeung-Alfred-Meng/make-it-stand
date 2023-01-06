#ifndef INNER_CARVING_CPP_MY_LBS_MATRIX_H
#define INNER_CARVING_CPP_MY_LBS_MATRIX_H
#include <Eigen/Core>

// Compute lbs matrix M s.t. V' = M * H,
// where V' is a vector of vertically stacked
// deformed 3d vertex positions,
// M is a #V' by 4 * #N matrix containing
// weights and vertices before deformation, and H
// is a 4 * N vector of vertically stacked translation
// and uniform scale factors for the transformation
// of each N handle. Order of factors is x-translation,
// y-translation, z-translation, uniform scale.
//
// Inputs:
//   V  #V by 3 vertex positions
//   W  #V by #N matrix of weights
//
// Output:
//   M  3 * #V by 4 * #N matrix lbs matrix
//
void my_lbs_matrix(
  const Eigen::VectorXd &V,
  const Eigen::MatrixXd &W,
  Eigen::MatrixXd &M);
#endif //INNER_CARVING_CPP_MY_LBS_MATRIX_H
