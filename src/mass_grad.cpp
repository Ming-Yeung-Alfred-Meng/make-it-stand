#include "mass_grad.h"

void mass_grad(
  const Eigen::MatrixXd &V,
  const Eigen::MatrixXi &F,
  Eigen::RowVectorXd &grad)
{
  grad.resize(V.rows());
  grad.setZero();

  // go over every face
  //  for each vertex, take the derivative of the face's contribution wrt that vertex
  //  add the derivative wrt each coordinate of the vertex into the appropriate slot of grad

  // and use mod to calculate indices into F.
  double grad_x;

  for (int i = 0; i < F.rows(); ++i) {
    grad_x = (V.row(F(i, 1)) - V.row(F(i, 0))).cross(
       V.row(F(i, 2)) - V.row(F(i, 0)))(0);

    for (int j = 0; j < 3; ++j) {
      grad(3 * F(i, j)) += grad_x;
      grad(3 * F(i, j) + 1) += () * ();
      grad(3 * F(i, j) + 2) += () * ();
    }
  }

}