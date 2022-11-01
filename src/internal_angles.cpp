#include "../include/internal_angles.h"
#include <cmath>

void internal_angles(
  const Eigen::MatrixXd & l_sqr,
  Eigen::MatrixXd & A)
{
  A.resizeLike(l_sqr);

  Eigen::MatrixXd l = l_sqr.cwiseSqrt();

  for (int i = 0; i < l_sqr.rows(); ++i) {
    for (int j = 0; j < 3; ++j) {
      int left = (j + 1) % 3;
      int right = (j + 2) % 3;
      A(i, j) = acos(
                               (l_sqr(i, j) - l_sqr(i, left) - l_sqr(i, right))
                               / (-2 * l(i, left) * l(i, right)));
    }
  }
}
