#include "mass.h"
#include <igl/centroid.h>

double mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &MiV,
  const Eigen::MatrixXi &MiF,
  const double density)
{
  double outer_volume;
  double inner_volume;
  Eigen::Vector3d dummy;

  igl::centroid(MoV, MoF, dummy, outer_volume);
  igl::centroid(MiV, MiF, dummy, outer_volume);

  return density * (outer_volume - inner_volume);
}


double mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const double density)
{
  double volume;
  Eigen::Vector3d dummy;

  igl::centroid(MoV, MoF, dummy, volume);

  return density * volume;
}
