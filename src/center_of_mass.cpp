#include "center_of_mass.h"
#include "mass.h"

void center_of_mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &MiV,
  const Eigen::MatrixXi &MiF,
  const double d,
  Eigen::Vector3d &C)
{
  // compute mass
  // compute c according to formular in paper

  C << 0., 0., 0.;

  double m = mass(MoV, MoF, MiV, MiF, d);

  for ()
}