#include "mass.h"
#include "iostream"

double face_contribution_to_mass(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F);


double mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &MiV,
  const Eigen::MatrixXi &MiF,
  const double density)
{
  return (face_contribution_to_mass(MoV, MoF) + face_contribution_to_mass(MiV, MiF)) * density / 6;
}


double mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const double density)
{
  return face_contribution_to_mass(MoV, MoF) * density / 6;
}


double face_contribution_to_mass(
  const Eigen::MatrixXd &V,
  const Eigen::MatrixXi &F)
{
  Eigen::Vector3d vi;
  Eigen::Vector3d vj;
  Eigen::Vector3d vk;
  double mass = 0;

  for (int i = 0; i < F.rows(); ++i) {
    vi = V.row(F(i, 0));
    vj = V.row(F(i, 1));
    vk = V.row(F(i, 2));
    mass += (vj - vi).cross(vk - vi)(0) * (vi + vj + vk)(0);
    // Reference the source code released by the authors of
    // the paper, Make It Stand: Balancing Shapes for 3D Fabrication.
  }

  return mass;
}
