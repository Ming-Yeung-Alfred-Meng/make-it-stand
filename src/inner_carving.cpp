#include <iostream>
#include <algorithm>
#include <vector>
#include <igl/voxel_grid.h>
#include <igl/centroid.h>
#include "inner_carving.h"
#include "voxel_indices.h"
#include "voxel_contouring.h"
#include "cube.h"
#include "face_contribution_to_com.h"


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
  std::cerr << "MoV.rows() & MoV.cols():\n" << MoV.rows() << " & " << MoV.cols() << std::endl;

  Eigen::MatrixXd grid;
  Eigen::RowVector3i side;
  igl::voxel_grid(MoV, 0, voxel_scale, 1, grid, side);

  std::cerr << "grid:\n" << grid << std::endl;

  const double length = grid(1, 0) - grid(0, 0);

  Eigen::Vector3d CoM;
  double mass;

  igl::centroid(MoV, MoF, CoM, mass);
  mass /= density;

  double energy = carving_energy(CoM, contact);

  std::vector<int> indices;
  voxel_indices(MoV, MoF, grid, thickness, indices);

  std::function<bool (int, int)> comp = [&](int i, int j)
  {
    return distance_from_plane(grid.row(i), contact, CoM)
           > distance_from_plane(grid.row(j), contact, CoM);
  };
  std::sort(indices.begin(), indices.end(), comp);

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

      std::cerr << "Current voxel being carved:\n" << grid.row(indices[j]) << std::endl;

      original_mass = mass;

      mass = reduce_mass_by_a_voxel(original_mass, density, length);
      update_center_of_mass(grid.row(indices[j]), length, original_mass, mass, density, CoM);
      energy = carving_energy(CoM, contact);

      if (energy < min_energy) {
        optimal_mass = mass;
        optimal_CoM = CoM;
        min_energy = energy;
        optimal_j = j;
      }
      ++j;
    }

//    assert (j < indices.size() && "The plane is outside the mesh!"); // Not sure if this is actually useful.
//    assert (optimal_j < indices.size() - 1 && "The entire mesh is now hollow!"); // Not sure if this is actually useful.
    std::cerr << (optimal_j < indices.size() - 1) << std::endl;
    if (optimal_j < ((int) indices.size()) - 1) {
      std::sort(indices.begin() + optimal_j + 1, indices.end(), comp);
    }
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
// Assumes the center of mass, CoM, passed into the
// function is valid.
//
// Inputs:
//   center  3D position of the voxel's center
//   length  length of each side of the (regular) voxel
//   old_mass  mass of the mesh before carving the voxel
//   new_mass  mass of the mesh after carving the voxel
//   density  density of the mesh
//
// Output:
//   CoM  updated center of mass
void update_center_of_mass(
  const Eigen::Vector3d &center,
  const double length,
  const double old_mass,
  const double new_mass,
  const double density,
  Eigen::Vector3d &CoM)
{
  Eigen::MatrixXd voxel_V;
  Eigen::MatrixXi voxel_F;
  cube(center, length, voxel_V, voxel_F);

  CoM *= (24 * old_mass) / density;

  Eigen::Vector3d con;
  face_contribution_to_com(voxel_V, voxel_F, con);
  CoM -= con;

  CoM *= density / (24 * new_mass);
}



double reduce_mass_by_a_voxel(
  const double old_mass,
  const double density,
  const double length)
{
  return old_mass - length * length * length * density;
}



// Compute relative (see note below) distance from a query point to a plane.
// The plane is defined to intersect "contact", and is perpendicular to the
// vector obtained by projecting CoM - contact onto the ground (the xy-plane).
//
// Inputs:
//   query  3D query point
//   contact  3D contact point of the object with the ground
//   CoM  3D current center of mass of the object
//
// Output:
//   relative distance from the query to the plane
//
// Note: all computed distances are relative because they are scaled by the length of
// the projection of CoM - contact onto the ground.
//
double distance_from_plane(
  const Eigen::Vector3d &query,
  const Eigen::Vector3d &contact,
  const Eigen::Vector3d &CoM)
{
  Eigen::Vector3d normal = CoM - contact;
  normal(2) = 0;
  return (query - contact).dot(normal);
}


// Should I pass in the iterators by reference of by value?
void build_in_out(
  const std::vector<int>::iterator &begin,
  const std::vector<int>::iterator &end,
  const long size,
  int in_out[])
{
  std::fill_n(in_out, size, 1);

  for (std::vector<int>::iterator i = begin; i < end; ++i) {
    in_out[*i] = -1;
  }
}
