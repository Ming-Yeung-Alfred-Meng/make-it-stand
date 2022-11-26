#include "cube.h"


void cube_V(const Eigen::Vector3d &center, const double length, Eigen::MatrixXd &V);
void cube_F(const Eigen::Vector3d &center, Eigen::MatrixXi &F);

void cube(
  const Eigen::Vector3d &center,
  const double length,
  Eigen::MatrixXd &V,
  Eigen::MatrixXi &F)
{
  cube_V(center, length, V);
  cube_F(center, F);
}


void cube_V(
  const Eigen::Vector3d &center,
  const double length,
  Eigen::MatrixXd &V)
{
  V.resize(8, 3);

  double half_length = length / 2;
  int n = 0;
  for (int k = -1; k <= 1; k += 2) {
    for (int j = -1; j <= 1; j += 2) {
      for (int i = -1; i <= 1; i += 2) {
        V.row(n) << center[0] + i * half_length, center[1] + j * half_length, center[2] + k * half_length;
        ++n;
      }
    }
  }
}


void cube_F(
  const Eigen::Vector3d &center,
  Eigen::MatrixXi &F)
{
  F.resize(12, 3);

  F << 0, 2, 1,
       1, 2, 3,
       2, 4, 6,
       0, 4, 2,
       0, 5, 4,
       0, 1, 5,
       3, 7, 5,
       1, 3, 5,
       2, 6, 7,
       2, 7, 3,
       4, 7, 6,
       4, 5, 7;
}