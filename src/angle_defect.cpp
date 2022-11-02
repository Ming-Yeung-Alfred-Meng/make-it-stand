#include "../include/angle_defect.h"
#include "internal_angles.h"
#include <igl/squared_edge_lengths.h>

void angle_defect(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::VectorXd & D)
{
  D = Eigen::VectorXd::Zero(V.rows());
  Eigen::MatrixXd l_sqr;
  Eigen::MatrixXd angles;

  igl::squared_edge_lengths(V, F, l_sqr);
  internal_angles(l_sqr, angles);

  for (int f = 0; f < F.rows(); ++f) {
    for (int v = 0; v < 3; ++v) {
      D(F(f, v)) += angles(f, v);
    }
  }

  D = Eigen::VectorXd::Constant(V.rows(), 2 * acos(-1.0)) - D;
}
