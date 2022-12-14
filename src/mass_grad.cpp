#include "mass_grad.h"

void mass_grad(
  const Eigen::MatrixXd &V,
  const Eigen::MatrixXi &F,
  const double density,
  Eigen::RowVectorXd &grad)
{
  grad.resize(3 * V.size());
  grad.setZero();

  double n_x;

  for (int i = 0; i < F.rows(); ++i) {
    n_x = (V.row(F(i, 1)) - V.row(F(i, 0))).cross(V.row(F(i, 2)) - V.row(F(i, 0)))(0);

    for (int j = 0; j < 3; ++j) {
      grad(3 * F(i, j)) += n_x;

      grad(3 * F(i, j) + 1) += (V(F(i, (j + 1) % 3), 2) - V(F(i, (j + 2) % 3), 2))
                               * (V(F(i, j), 0) + V(F(i, (j + 1) % 3), 0) + V(F(i, (j + 2) % 3), 0));

      grad(3 * F(i, j) + 2) += (V(F(i, (j + 2) % 3), 1) - V(F(i, (j + 1) % 3), 1))
                               * (V(F(i, j), 0) + V(F(i, (j + 1) % 3), 0) + V(F(i, (j + 2) % 3), 0));
    }
  }

  grad *= density / 6;
}