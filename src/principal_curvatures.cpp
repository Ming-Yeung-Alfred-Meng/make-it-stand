#include "../include/principal_curvatures.h"
#include <igl/adjacency_list.h>
#include <unordered_set>
#include <cmath>
#include <igl/sort.h>


void set_insert(
  const std::vector<int> & vector,
  std::unordered_set<int> & u_set)
{
  for (int vi : vector) {
    u_set.insert(vi);
  }
}


void indices_to_matrix(
  const Eigen::MatrixXd & V,
  const std::unordered_set<int> & tworing_i,
  Eigen::MatrixXd & tworing)
{
  std::vector<int> index_l(tworing_i.begin(), tworing_i.end());
  tworing.resize((long) index_l.size(), 3);

  for (int vi : index_l) {
    tworing.block<1, 3>(vi, 0) = V.row(vi);
  }
}


// Build k by 3 tworing.
void build_tworing(
  const Eigen::MatrixXd & V,
  const std::vector<std::vector<int>> & adjacency_l,
  const int v,
  Eigen::MatrixXd & tworing)
{
  std::unordered_set<int> tworing_i;

  tworing_i.insert(v);
  set_insert(adjacency_l[v], tworing_i);

  for (int i : adjacency_l[v]) {
    set_insert(adjacency_l[i], tworing_i);
  }

  indices_to_matrix(V, tworing_i, tworing);
  tworing.rowwise() -= V.row(v);
}

// may need to change the paramerter name of "change of basis", it is confusing, since it is the inverse
// of what it is supposed to mean. May be add a bool parameter that indicate whether it is the inverse or not.
void world_to_local(
  const Eigen::MatrixXd & change_of_basis,
  const Eigen::MatrixXd & A,
  Eigen::MatrixXd & B)
{
//  B = change_of_basis * A.transpose();
  B = A * change_of_basis;
}

// coeff: k x 5
// k is the number of rows in tworing_local
// tworing_local: k x 3 (u, v, w)
void build_quadratic_coeff(
  const Eigen::MatrixXd & tworing_local,
  Eigen::MatrixXd & coeff)
{
  // resize coeff to k x 5
  // loop each row of tworing_local,
  // use the first two column of tworing_local to fill in coeff

  coeff.resize(tworing_local.rows(), 5);

  for (int i = 0; i < tworing_local.rows(); ++i) {
    coeff(i, 0) = tworing_local(i, 0);
    coeff(i, 1) = tworing_local(i, 1);
    coeff(i, 2) = pow(tworing_local(i, 0), 2);
    coeff(i, 3) = tworing_local(i, 0) * tworing_local(i, 1);
    coeff(i, 4) = pow(tworing_local(i, 1), 2);
  }
}


// a: 5 vector
// S_left: 2 x 2 matrix
void build_S_left(
  const Eigen::VectorXd & a,
  Eigen::Matrix2d & S_left)
{
  double denominator = sqrt(pow(a[0], 2) + pow(a[1], 2) + 1);
  S_left(0, 0) =  2 * a[2] / denominator;
  S_left(0, 1) = a[3] / denominator;
  S_left(1, 0) = S_left(0, 1);
  S_left(1, 1) = 2 * a[4] / denominator;
}


void build_S_right(
  const Eigen::VectorXd & a,
  Eigen::Matrix2d & S_right)
{
  S_right(0, 0) = 1 + pow(a[0], 2);
  S_right(0, 1) = a[0] * a[1];
  S_right(1, 0) = S_right(0, 1);
  S_right(1, 1) = 1 + pow(a[1], 2);
}


void build_shape_operator(
  const Eigen::MatrixXd & V,
  const std::vector<std::vector<int>> & adjacency_l,
  const int v,
  Eigen::Matrix2d & S,
  Eigen::Matrix3d & r_sgl_vectors)
{
  // i.e. find k x 3 tworing,
  // svd decompose tworing
  // fit curve
  // build shape operator
  Eigen::MatrixXd tworing;
  Eigen::MatrixXd tworing_local;
  Eigen::MatrixXd coeff;
  Eigen::MatrixXd p_inverse;
  Eigen::VectorXd a;
  Eigen::Matrix2d S_left;
  Eigen::Matrix2d S_right;

  build_tworing(V, adjacency_l, v, tworing);

  Eigen::JacobiSVD<Eigen::MatrixXd> svd(tworing, Eigen::ComputeThinV);  // svd.singularValues() sorted in decreasing order.

  world_to_local(svd.matrixV(), tworing, tworing_local);

  build_quadratic_coeff(tworing_local, coeff);

  p_inverse = coeff.completeOrthogonalDecomposition().pseudoInverse();

  a = p_inverse * tworing_local.col(2); // check if tworing or tworing local.

  build_S_left(a, S_left);
  build_S_right(a, S_right);

  S = - (S_left * S_right.inverse());
  r_sgl_vectors = svd.matrixV();
}


void local_to_world(
  const Eigen::Matrix<double, 3, 2> & change_of_basis,
  const Eigen::Matrix2d & local,
  Eigen::Matrix<double, 3, 2> & world)
{
  world = change_of_basis * local;
}


void principal_curvatures(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & D1,
  Eigen::MatrixXd & D2,
  Eigen::VectorXd & K1,
  Eigen::VectorXd & K2)
{
  Eigen::Matrix2d S;
  std::vector<std::vector<int>> adjacency_l;
  Eigen::Vector2d curvatures;
  Eigen::Vector2i directions_i;
  Eigen::Matrix<double, 3, 2> directions_world;
  Eigen::Matrix3d r_sgl_vectors;

  K1.resize(V.rows());
  K2.resize(V.rows());
  D1.resize(V.rows(), 3);
  D2.resize(V.rows(), 3);

  igl::adjacency_list(F, adjacency_l, false);

  for (int v = 0; v < V.rows(); ++v) {
    build_shape_operator(V, adjacency_l, v, S, r_sgl_vectors);

    // what if eigenvalues are complex.
    Eigen::EigenSolver<Eigen::Matrix2d> eigensolver(S);
    if (eigensolver.info() != Eigen::Success) abort();

    igl::sort(eigensolver.eigenvalues().real(), 1, false, curvatures, directions_i);

    local_to_world(r_sgl_vectors.block<3, 2>(0, 0), eigensolver.eigenvectors().real(), directions_world);

    K1(v) = curvatures(0); // max
    K2(v) = curvatures(1);

    for (int i = 0; i < 3; ++i) {
      D1(v, i) = directions_world(i, directions_i(0));
      D2(v, i) = directions_world(i, directions_i(1));
    }
  }
}
