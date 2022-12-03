#include "laplacian.h"
#include <igl/cotmatrix_entries.h>


void add_block(const int i, const int j, const double value, std::vector<Eigen::Triplet<double>> &triplet_list);


void laplacian(
  const Eigen::MatrixXd &V,
  const Eigen::MatrixXi &F,
  Eigen::SparseMatrix<double> &Lap)
{
  Eigen::MatrixXd Lap_entries;
  igl::cotmatrix_entries(V, F, Lap_entries);

  std::vector<Eigen::Triplet<double>> triplet_list;
  triplet_list.reserve(F.rows() * 12);

  for (int i = 0; i < F.rows(); ++i) {
    for (int j = 0; j < 3; ++j) {
      add_block(F(i, j), F(i, (j + 1) % 3), Lap_entries(i, (j + 2) % 3), triplet_list);
      add_block(F(i, (j + 1) % 3), F(i, j), Lap_entries(i, (j + 2) % 3), triplet_list);
      add_block(F(i, j), F(i, j), -Lap_entries(i, (j + 2) % 3), triplet_list);
      add_block(F(i, (j + 1) % 3), F(i, (j + 1) % 3), -Lap_entries(i, (j + 2) % 3), triplet_list);
    }
  }
}


void add_block(
  const int i,
  const int j,
  const double value,
  std::vector<Eigen::Triplet<double>> &triplet_list)
{
  int top_left_i = 3 * i;
  int top_left_j = 3 * j;

  triplet_list.emplace_back(top_left_i, top_left_j, value);
  triplet_list.emplace_back(top_left_i + 1, top_left_j + 1, value);
  triplet_list.emplace_back(top_left_i + 2, top_left_j + 2, value);
}