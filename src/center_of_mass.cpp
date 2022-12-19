#include "center_of_mass.h"
#include "mass.h"
#include "helper_g.h"
#include "face_contribution_to_com.h"

void center_of_mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &MiV,
  const Eigen::MatrixXi &MiF,
  const double density,
  Eigen::Vector3d &CoM,
  double &m)
{
  assert(MoV.cols() == 3);
  assert(MoV.cols() == 3);

  Eigen::Vector3d outer_con;
  Eigen::Vector3d inner_con;
  m = mass(MoV, MoF, MiV, MiF, density);

  face_contribution_to_com(MoV, MoF, outer_con);
  face_contribution_to_com(MiV, MiF, inner_con);

  CoM = (density / (24 * m)) * (outer_con + inner_con);
}


void center_of_mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const double density,
  Eigen::Vector3d &CoM,
  double &m)
{
  assert(MoV.cols() == 3);
  assert(MoV.cols() == 3);

  Eigen::Vector3d con;
  m = mass(MoV, MoF, density);

  face_contribution_to_com(MoV, MoF, con);
  CoM = (density / (24 * m)) * con;
}