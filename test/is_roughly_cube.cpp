#include <cmath>
#include "is_roughly_cube.h"


bool is_roughly_cube(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const Eigen::Vector3d & center,
  const double side)
{
  Eigen::Vector3d min_corner = V.colwise().minCoeff();
  Eigen::Vector3d max_corner = V.colwise().maxCoeff();

  Eigen::Vector3d diagonal = max_corner - min_corner;
  Eigen::Vector3d proj_diagonal = diagonal;
  proj_diagonal(2) = 0;

  return ((min_corner + 0.5 * diagonal).isApprox(center) &&
          fabs(diagonal.norm() - sqrt(3) * side) < 1e-10 &&
          fabs(acos(diagonal.dot(proj_diagonal) / (diagonal.norm() * proj_diagonal.norm())) - atan2(1, sqrt(2))) < 1e-10);
}