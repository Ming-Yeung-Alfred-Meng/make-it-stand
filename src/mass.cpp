#include "mass.h"

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
  double mass = 0;
  for (int i = 0; i < F.rows(); ++i) {
    mass += ((V.row(F(i, 1)) - V.row(F(i, 0))).cross(
      V.row(F(i, 2)) - V.row(F(i, 0)))).dot(V.row(F(i, 0))
                                                                                   + V.row(F(i, 1))
                                                                                   + V.row(F(i, 2)));
  }

  return mass;
}
