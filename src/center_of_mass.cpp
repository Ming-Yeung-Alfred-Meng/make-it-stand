#include "center_of_mass.h"
#include "mass.h"


// The function g from equation 4 of the paper.
//
// Inputs:
//    vi first vertex of a face
//    vj second vertex of a face
//    vk third vertex of a face
//
// Output:
//    g expression from equation 4 of the paper
//
void helper_g(
  const Eigen::Vector3d &vi,
  const Eigen::Vector3d &vj,
  const Eigen::Vector3d &vk,
  Eigen::Vector3d &g)
{
  g = (vi.array() * vi.array())
      + (vi.array() * vj.array())
      + (vj.array() * vj.array())
      + (vj.array() * vk.array())
      + (vk.array() * vk.array())
      + (vk.array() * vi.array());
}


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
  const double p,
  Eigen::Vector3d &C)
{
  Eigen::Vector3d outer_con;
  Eigen::Vector3d inner_con;

  face_contribution(MoV, MoF, outer_con);
  face_contribution(MiV, MiF, inner_con);

  C = (p / (24 * mass(MoV, MoF, MiV, MiF, p))) * (outer_con + inner_con);

  // compute mass
  // compute c according to formula in paper
}


void center_of_mass(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const double p,
  Eigen::Vector3d &C)
{
  Eigen::Vector3d con;

  face_contribution(MoV, MoF, con);
  C = (p / (24 * mass(MoV, MoF, p))) * con;

  // compute mass
  // compute c according to formula in paper
}