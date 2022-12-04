#include "center_of_mass_grad.h"
#include "mass.h"
#include "mass_grad.h"
#include "center_of_mass.h"
#include "helper_g.h"


void mass_center_of_mass_grad(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F, const double density, Eigen::MatrixXd &grad);


void center_of_mass_grad(
  const Eigen::MatrixXd &dV,
  const Eigen::MatrixXi &dF,
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &MiV,
  const Eigen::MatrixXi &MiF,
  const double density,
  Eigen::MatrixXd &grad)
{
  Eigen::RowVectorXd m_grad;
  Eigen::MatrixXd mc_grad;
  double m;
  Eigen::Vector3d CoM;

  mass_grad(dV, dF, density, m_grad);
  mass_center_of_mass_grad(dV, dF, density, mc_grad);
  center_of_mass(MoV, MoF, MiV, MiF, density, CoM, m);

  grad = (mc_grad - CoM * m_grad) / m;
}


void mass_center_of_mass_grad(
  const Eigen::Matrix<double, Eigen::Dynamic, 3> &V,
  const Eigen::MatrixXi &F,
  const double density,
  Eigen::MatrixXd &grad)
{
  grad.resize(3, V.size());
  grad.setZero();

  Eigen::Vector3d n;
  Eigen::Vector3d g;
  Eigen::Vector3d vi;
  Eigen::Vector3d vj;
  Eigen::Vector3d vk;

  for (int i = 0; i < F.rows(); ++i) {
    n = (V.row(F(i, 1)) - V.row(F(i, 0))).cross(V.row(F(i, 2)) - V.row(F(i, 0)));
    helper_g(V.row(F(i, 0)), V.row(F(i, 1)), V.row(F(i, 2)), g);

    for (int j = 0; j < 3; ++j) {
      vi = V.row(F(i, j));
      vj = V.row(F(i, (j + 1) % 3));
      vk = V.row(F(i, (j + 2) % 3));

      grad.block(0, 3 * F(i, j), 3, 3) += (Eigen::MatrixXd(3, 3) << n(0) * (2 * vi(0) + vj(0) + vk(0)),
                                                                    g(0) * (vj(2) - vk(2)),
                                                                    g(0) * (vk(1) - vj(1)),
                                                                    g(1) * (vj(2) - vk(2)),
                                                                    n(1) * (2 * vi(1) + vj(1) + vk(1)),
                                                                    g(1) * (vk(0) - vj(0)),
                                                                    g(2) * (vj(1) - vk(1)),
                                                                    g(2) * (vk(0) - vj(0)),
                                                                    n(2) * (2 * vi(2) + vj(2) + vk(2))).finished();
    }
  }

  grad *= density / 24;
}