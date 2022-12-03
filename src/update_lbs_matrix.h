#ifndef INNER_CARVING_CPP_UPDATE_LBS_MATRIX_H
#define INNER_CARVING_CPP_UPDATE_LBS_MATRIX_H
#include <Eigen/Core>

void update_lbs_matrix(
  const Eigen::VectorXd &V,
  const Eigen::MatrixXd &W,
  Eigen::MatrixXd &M);
#endif //INNER_CARVING_CPP_UPDATE_LBS_MATRIX_H
