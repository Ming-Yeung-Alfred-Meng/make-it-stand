#include <algorithm>
#include <cmath>
#include <vector>
#include <igl/voxel_grid.h>
#include <igl/dual_contouring.h>
#include "inner_carving.h"
#include "center_of_mass.h"
#include "voxel_indices.h"
#include "isovoxel.h"
#include "helper_g.h"
#include "voxel_contouring.h"
#include "cube.h"
#include "face_contribution_to_com.h"


double carving_energy(const Eigen::Vector3d &CoM, const Eigen::Vector3d &contact);
void update_center_of_mass(const Eigen::MatrixXd &grid, const int i, const double old_mass, const double new_mass, Eigen::Vector3d &CoM);
double update_mass(const double old_mass, const double density);
void build_in_out(const std::vector<int>::iterator &begin, const std::vector<int>::iterator &end, const long size, int in_out[]);
std::function<bool (int, int)> generate_comp(const Eigen::MatrixXd &grid, const Eigen::Vector3d &contact, const Eigen::Vector3d &CoM);
double distance_from_plane(const Eigen::Vector3d &query, const Eigen::Vector3d &contact, const Eigen::Vector3d &CoM);


void inner_carving(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::Vector3d &contact,
  const int voxel_scale,
  const int min_carve,
  const double thickness,
  const double density,
  Eigen::MatrixXd &MiV,
  Eigen::MatrixXi &MiF)
{
  Eigen::MatrixXd grid;
  Eigen::RowVector3i side;
  igl::voxel_grid(MoV, 0, voxel_scale, 1, grid, side);

  Eigen::Vector3d CoM;
  double mass;
  center_of_mass(MoV, MoF, density, CoM, mass);

  double energy = carving_energy(CoM, contact);

  std::vector<int> indices;
  voxel_indices(MoV, MoF, grid, thickness, indices);

  std::sort(indices.begin(), indices.end(), generate_comp(grid, contact, CoM));

  double original_mass; // What should I initialize them to?
  double optimal_mass = mass;
  Eigen::Vector3d original_CoM;
  Eigen::Vector3d optimal_CoM = CoM;
  double original_energy;
  double min_energy = energy;
  int j = 0;
  int original_j;
  int optimal_j = 0;
  // while "indices" contains indices into grid, "j", "original_j" and "optimal_j" are indices into "indices".

  do {
    mass = optimal_mass; // may place below or here.
    original_energy = min_energy; // must be here.
    original_CoM = optimal_CoM; // may be moved below, but need to be initialized before do-while.
    CoM = optimal_CoM;
    original_j = optimal_j; // must be here

    while (j < indices.size() && distance_from_plane(grid.row(indices[j]), contact, original_CoM) > 0) {
      original_mass = mass;
      mass = update_mass(original_mass, density);
      update_center_of_mass(grid, indices[j], original_mass, mass, CoM);
      energy = carving_energy(CoM, contact);

      if (energy < min_energy) {
        optimal_mass = mass;
        optimal_CoM = CoM;
        min_energy = energy;
        optimal_j = j;
      }

      ++j;
    }

    assert (j < indices.size() && "The plane is outside the mesh!");
    assert (optimal_j < indices.size() - 1 && "The entire mesh is now hollow!");

    std::sort(indices.begin() + optimal_j + 1, indices.end(), generate_comp(grid, contact, optimal_CoM));

  } while (min_energy < original_energy && optimal_j - original_j > min_carve);

  int in_out[grid.rows()];
  build_in_out(indices.begin(), indices.begin() + optimal_j, grid.rows(), in_out);

  voxel_contouring(grid, side, in_out, MiV, MiF);
}


// Compute the E_CoM from paper.
//
// For simplicity, assume the negative z-direction
// is the gravity direction.
//
// Inputs:
//   CoM  center of mass
//   contact  mesh's point of contact with the ground.
//            For simplicity, assumed to be the vertex
//            with the smallest z-value, or is given by user.
// Return:
//   E_CoM
double carving_energy(
  const Eigen::Vector3d &CoM,
  const Eigen::Vector3d &contact)
{
  Eigen::Vector3d diff = CoM - contact;
  return 0.5 * (diff(0) * diff(0) + diff(1) * diff(1));
}


// Update the center of mass after carving one voxel.
// Assumes the old center of mass and mass are valid.
//
// Inputs:
//   grid  #grid by 3 list of voxel centers
//   i  index into grid of voxel carved
//   CoM  old center of mass
//   mass  old mass
//
// Output:
//   CoM  updated center of mass
//   mass  updated mass
void update_center_of_mass(
  const Eigen::MatrixXd &grid,
  const int i,
  const double old_mass,
  const double new_mass,
  const double density,
  Eigen::Vector3d &CoM)
{
  Eigen::MatrixXd voxel_V;
  Eigen::MatrixXi voxel_F;
  cube(grid.row(i), grid(1, 0) - grid(0, 0), voxel_V, voxel_F);

  CoM *= (24 * old_mass) / density;

  Eigen::Vector3d con;
  face_contribution_to_com(voxel_V, voxel_F, con);
  CoM -= con;

  CoM *= density / (24 * new_mass);
}



double update_mass(
  const double old_mass,
  const double density)
{

}



// Compute relative distance from grid.row(index) to the plane.
// Relative because the normal of the plane does not have unit length.
double distance_from_plane(
  const Eigen::Vector3d &query,
  const Eigen::Vector3d &contact,
  const Eigen::Vector3d &CoM)
{
  Eigen::Vector3d normal = CoM - contact;
  normal(2) = 0;
//  return (grid.row(index).transpose() - contact).dot(normal);
  return (query - contact).dot(normal);
}



// Return a function that compares the distance from two voxels to a plane.
// The function returns true iff the first voxel is closer to the plane.
//
// Inputs:
//   grid  #grid by 3 list of voxel centers
//   contact  3D vector that is on the plane
//   CoM  3D vector s.t. (CoM - contact) projected on the ground is normal to the plane
//
// Output:
//   comparator
std::function<bool (int, int)> generate_comp(
  const Eigen::MatrixXd &grid,
  const Eigen::Vector3d &contact,
  const Eigen::Vector3d &CoM)
{
  return [&](int i, int j)
    { return distance_from_plane(grid.row(i), contact, CoM)
             > distance_from_plane(grid.row(j), contact, CoM);
    };
}


// Should I pass in the iterators by reference of by value?
void build_in_out(
  const std::vector<int>::iterator &begin,
  const std::vector<int>::iterator &end,
  const long size,
  int in_out[])
{
  std::fill_n(in_out, size, 1);

  for (std::vector<int>::iterator i = begin; i != end; ++i) {
    in_out[*i] = -1;
  }
}
