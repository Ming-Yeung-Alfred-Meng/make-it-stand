#include "../include/mean_curvature.h"
#include <igl/cotmatrix.h>
#include <igl/massmatrix.h>
#include <igl/per_vertex_normals.h>


void mean_curvature(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::VectorXd & H)
{
  Eigen::SparseMatrix<double> L;
  Eigen::SparseMatrix<double> M;
  Eigen::MatrixXd N;
  Eigen::MatrixXd X;

  igl::cotmatrix(V, F, L);
  igl::massmatrix(V, F, igl::MASSMATRIX_TYPE_BARYCENTRIC, M);
  igl::per_vertex_normals(V, F, N);

  Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> solver;
  solver.compute(M);

  if (solver.info() != Eigen::Success) {
    exit(EXIT_FAILURE);
  }

  X = solver.solve((-L) * V);

  if (solver.info() != Eigen::Success) {
    exit(EXIT_FAILURE);
  }

  H = X.cwiseProduct(N).rowwise().sum();
}
