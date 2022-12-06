#include "deformation.h"
#include "center_of_mass.h"
#include "center_of_mass_grad.h"
#include "my_lbs_matrix.h"
#include "laplacian.h"


void deformation(
  const Eigen::Vector3d &contact,
  const double lambda_init,
  const double mu_init,
  const double step_size_init,
  const int max_iter,
  const double min_change,
  const bool verbose,
  Eigen::MatrixXd &MoV,
  Eigen::MatrixXi &MoF,
  Eigen::MatrixXd &MiV,
  Eigen::MatrixXi &MiF)
{
  // perform gradient descent to minimize energy
  double lambda = lambda_init;
  double mu = mu_init;
  double step_size = step_size_init;
  Eigen::VectorXd H;
  // initialize H to random values
  // use H to transform MoV and MiV
  // calculate the gradient
  // if none of thresholds met


  Eigen::SparseMatrix<double> MoLap;
  Eigen::MatrixXd Molbs;
  Eigen::MatrixXd Milbs;

//  my_lbs_matrix(MoV, MoW, Molbs);
//  my_lbs_matrix(MiV, MiW, Milbs);
  laplacian(MoV, MoF, MoLap);
  // remeber to negate MoLap

//  center_of_mass_grad()
}