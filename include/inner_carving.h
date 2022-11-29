#ifndef CURVATURE_INNER_CARVING_H
#define CURVATURE_INNER_CARVING_H
#include <Eigen/Core>

void inner_carving(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::Vector3d &contact,
  const int voxel_scale,
  const int min_carve,
  const double thickness,
  const double density,
  Eigen::MatrixXd &MiV,
  Eigen::MatrixXi &MiF);
#endif //CURVATURE_INNER_CARVING_H
