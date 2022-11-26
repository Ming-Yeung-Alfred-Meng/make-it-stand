#include "center_of_mass.h"
#include "mass.h"
#include "helper_g.h"

// TODO: fix it due to inner mesh's normals point inward
// Compute the contribution of faces of a (outer or inner) mesh
// to the center of mass of the mesh defined collectively by the
// outer and inner mesh.
//
// Inputs:
//   V vertices
//   F faces
//
// Output:
//   con contribution
//
void face_contribution(
  const Eigen::MatrixXd &V,
  const Eigen::MatrixXi &F,
  Eigen::Vector3d &con)
{
  Eigen::Vector3d vi;
  Eigen::Vector3d vj;
  Eigen::Vector3d vk;
  Eigen::Vector3d g;

  con << 0., 0., 0.;
  for (int i = 0; i < F.rows(); ++i) {
    vi = V.row(F(i, 0));
    vj = V.row(F(i, 1));
    vk = V.row(F(i, 2));
    helper_g(vi, vj, vk, g);
    con += ((vj - vi).cross(vk - vi).array() * g.array()).matrix();
  }

  // loop through each face
  //    fetch each vertex of the face from V
  //    compute contribution
  //    add it to con
}


void center_of_mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::MatrixXd &MiV,
  const Eigen::MatrixXi &MiF,
  const double density,
  Eigen::Vector3d &CoM,
  double &m)
{
  Eigen::Vector3d outer_con;
  Eigen::Vector3d inner_con;
  m = mass(MoV, MoF, MiV, MiF, density);

  face_contribution(MoV, MoF, outer_con);
  face_contribution(MiV, MiF, inner_con);

  CoM = (density / (24 * m)) * (outer_con + inner_con);

  // compute mass
  // compute c according to formula in paper
}


void center_of_mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const double density,
  Eigen::Vector3d &CoM,
  double &m)
{
  Eigen::Vector3d con;
  m = mass(MoV, MoF, density);

  face_contribution(MoV, MoF, con);
  CoM = (density / (24 * m)) * con;

  // compute mass
  // compute c according to formula in paper
}