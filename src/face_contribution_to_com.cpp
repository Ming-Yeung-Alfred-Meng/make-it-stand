#include "face_contribution_to_com.h"
#include "helper_g.h"


void face_contribution_to_com(
  const Eigen::MatrixXd &V,
  const Eigen::MatrixXi &F,
  Eigen::Vector3d &con)
{
  Eigen::Vector3d vi;
  Eigen::Vector3d vj;
  Eigen::Vector3d vk;
  Eigen::Vector3d g;

  con << 0., 0., 0.;
  for (int i = 0; i < F.rows(); ++i) {
    vi = V.row(F(i, 0));
    vj = V.row(F(i, 1));
    vk = V.row(F(i, 2));
    helper_g(vi, vj, vk, g);
    con += (((vj - vi).cross(vk - vi)).array() * g.array()).matrix();
  }
}
