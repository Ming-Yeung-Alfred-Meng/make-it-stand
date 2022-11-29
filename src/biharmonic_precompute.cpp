#include "biharmonic_precompute.h"
#include <igl/min_quad_with_fixed.h>
#include <igl/cotmatrix.h>
#include <igl/massmatrix.h>
#include <igl/invert_diag.h>

void compute_quadratic_coeff(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::SparseMatrix<double> & quadratic_coeff)
{
  Eigen::SparseMatrix<double> L, M, M_inv;
  igl::cotmatrix(V, F, L);
  igl::massmatrix(V, F, igl::MASSMATRIX_TYPE_BARYCENTRIC, M);
  igl::invert_diag(M, M_inv);

  quadratic_coeff = L.transpose() * M_inv * L;
}


void biharmonic_precompute(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const Eigen::VectorXi & b,
  igl::min_quad_with_fixed_data<double> & data)
{
  // I wonder if not setting the dimentions of quadratic_coeff is going to work.
  // Experiment: uninitialized "empty" variable or all zero "empty" variable.
  Eigen::SparseMatrix<double> quadratic_coeff;
  Eigen::SparseMatrix<int> empty;
  compute_quadratic_coeff(V, F, quadratic_coeff);

  igl::min_quad_with_fixed_precompute(quadratic_coeff, b, Eigen::SparseMatrix<double>(), true, data);
}

// What does it mean for a matrix to be empty?
  // unintialized matrix variable?
  // initialized by has no content?
  // Does B has to be a matrix of zero?