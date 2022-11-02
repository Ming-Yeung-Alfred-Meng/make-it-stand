#include "../include/principal_curvatures.h"
#include <igl/adjacency_list.h>
#include <unordered_set>


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


void change_basis(
  const Eigen::MatrixXd & change_of_basis,
  const Eigen::MatrixXd & A,
  Eigen::MatrixXd & B)
{
//  B = change_of_basis * A.transpose();
  B = A * change_of_basis;
}


void build_quadratic_coeff(
  const Eigen::MatrixXd & tworing_local,
  Eigen::MatrixXd & coeff)
{

}

void build_S_left(
  const Eigen::VectorXd & a,
  Eigen::Matrix2d & S_left)
{

}

void build_S_right(
  const Eigen::VectorXd & a,
  Eigen::Matrix2d & S_right)
{

}


void build_shape_operator(
  const Eigen::MatrixXd & V,
  const std::vector<std::vector<int>> & adjacency_l,
  const int v,
  Eigen::Matrix2d & S)
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

  Eigen::JacobiSVD<Eigen::MatrixXd> svd(tworing, Eigen::ComputeThinV);
  //  svd.singularValues(); //decreasing order.

//  change_basis(svd.matrixV().inverse(), tworing, tworing_local);
  change_basis(svd.matrixV(), tworing, tworing_local);

  build_quadratic_coeff(tworing_local, coeff);

  p_inverse = coeff.completeOrthogonalDecomposition().pseudoInverse();

  a = p_inverse * tworing_local.col(2); // check if tworing or tworing local.

  build_S_left(a, S_left);
  build_S_right(a, S_right);

  S = - (S_left * S_right.inverse());
}


void principal_curvatures(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & D1,
  Eigen::MatrixXd & D2,
  Eigen::VectorXd & K1,
  Eigen::VectorXd & K2)
{
  // for every vertex
  // compute shape operator:
  // i.e. find k x 3 neighbourhood,
  // svd decompose neighbourhood
  // fit curve
  // build shape operator
  // eigen decompose shape operator
  // shift points
  Eigen::Matrix2d S;
  std::vector<std::vector<int>> adjacency_l;

  igl::adjacency_list(F, adjacency_l, false);

  for (int v = 0; v < V.rows(); ++v) {
    build_shape_operator(V, adjacency_l, v, S);
//    eigen_decompose();
//    map_to_world();
  }

  K1 = Eigen::VectorXd::Zero(V.rows());
  K2 = Eigen::VectorXd::Zero(V.rows());
  D1 = Eigen::MatrixXd::Zero(V.rows(),3);
  D2 = Eigen::MatrixXd::Zero(V.rows(),3);
}
