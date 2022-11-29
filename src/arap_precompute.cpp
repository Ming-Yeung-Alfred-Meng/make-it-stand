#include "arap_precompute.h"
#include <igl/min_quad_with_fixed.h>
#include <igl/arap_linear_block.h>
#include <igl/cotmatrix.h>


void build_K(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::SparseMatrix<double> & K)
{
  K.resize(V.rows(), 3 * V.rows());

  std::vector<Eigen::Triplet<double>> triplet_list;
  triplet_list.reserve(F.rows() * 54);
  Eigen::Vector3d e;
  Eigen::MatrixXd cotmatrix;
  igl::cotmatrix_entries(V, F, cotmatrix);

  for (int i = 0; i < F.rows(); ++i) {
    for (int j = 0; j < 3; ++j) {
      // We may or may not need to multiply e by 2 here:
      e = cotmatrix(i, (j + 2) % 3) * (V.row(F(i, j)) - V.row(F(i, (j + 1) % 3)));
      for (int k = 0; k < 3; ++k) {
        for (int l = 0; l < 3; ++l) {
          triplet_list.emplace_back(F(i, j), 3 * F(i, k) + l, e(l));
          triplet_list.emplace_back(F(i, (j + 1) % 3), 3 * F(i, k) + l, - e(l));
        }
      }
    }
  }

  K.setFromTriplets(triplet_list.begin(), triplet_list.end());
}


void arap_precompute(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const Eigen::VectorXi & b,
  igl::min_quad_with_fixed_data<double> & data,
  Eigen::SparseMatrix<double> & K)
{
  Eigen::SparseMatrix<double> L;
  igl::cotmatrix(V, F, L);
  L *= -1;
  L /= 2;

  build_K(V, F, K);
  igl::min_quad_with_fixed_precompute(L, b, Eigen::SparseMatrix<double>(), true, data);
}
