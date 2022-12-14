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
  const Eigen::Vector3d &down,
  const Eigen::Vector3d &contact,
  const int voxel_scale,
  const int min_carve,
  const double thickness,
  const double density,
  Eigen::MatrixXd &MiV,
  Eigen::MatrixXi &MiF)
{
//  std::cerr << "MoV.rows() & MoV.cols():\n" << MoV.rows() << " & " << MoV.cols() << std::endl;

  Eigen::MatrixXd grid;
  Eigen::RowVector3i side;
  igl::voxel_grid(MoV, 0, voxel_scale, 1, grid, side);

//  std::cerr << "grid:\n" << grid << std::endl;

  const double length = grid(1, 0) - grid(0, 0);

  Eigen::Vector3d CoM;
  double mass;

  igl::centroid(MoV, MoF, CoM, mass);
  mass /= density;

  double energy = carving_energy(down, contact, CoM);

  std::vector<int> indices;
  voxel_indices(MoV, MoF, grid, thickness, indices);

  std::function<bool (int, int)> comp = [&](int i, int j)
  {
    return distance_from_plane(grid.row(i), down, contact, CoM)
           > distance_from_plane(grid.row(j), down, contact, CoM);
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

    while (j < indices.size() && distance_from_plane(grid.row(indices[j]), down, contact, original_CoM) > 0) {

//      std::cerr << "Current voxel being carved:\n" << grid.row(indices[j]) << std::endl;

      original_mass = mass;

      mass = reduce_mass_by_a_voxel(original_mass, density, length);
      update_center_of_mass(grid.row(indices[j]), length, original_mass, mass, density, CoM);
      energy = carving_energy(down, contact, CoM);

      if (energy < min_energy) {
        optimal_mass = mass; // Only assigned, never used. Wait it is actually used in the next iteration.
        optimal_CoM = CoM; // Only assigned, never used. Wait it is actually used in the next iteration.
        min_energy = energy;
        optimal_j = j;
      }
      ++j;
    }

//    assert (j < indices.size() && "The plane is outside the mesh!"); // Not sure if this is actually useful.
//    assert (optimal_j < indices.size() - 1 && "The entire mesh is now hollow!"); // Not sure if this is actually useful.
//    std::cerr << (optimal_j < indices.size() - 1) << std::endl;
    if (optimal_j < ((int) indices.size()) - 1) {
      std::sort(indices.begin() + optimal_j + 1, indices.end(), comp);
    }
  } while (min_energy < original_energy && optimal_j - original_j > min_carve);

  int carved_mask[grid.rows()];
  build_carved(indices.begin(), indices.begin() + optimal_j, grid.rows(), carved_mask);

  voxel_contouring(grid, side, carved_mask, MiV, MiF);
}


double carving_energy(
  const Eigen::Vector3d &down,
  const Eigen::Vector3d &contact,
  const Eigen::Vector3d &CoM)
{
  Eigen::Vector3d normal;
  floor_projection(CoM - contact, down, normal);
  return normal.squaredNorm();
}


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

  Eigen::Vector3d con;
  face_contribution_to_com(voxel_V, voxel_F, con);

  CoM = CoM * old_mass / new_mass - con * density / (24 * new_mass);
}


double reduce_mass_by_a_voxel(
  const double old_mass,
  const double density,
  const double length)
{
  return old_mass - length * length * length * density;
}


double distance_from_plane(
  const Eigen::Vector3d &query,
  const Eigen::Vector3d &down,
  const Eigen::Vector3d &contact,
  const Eigen::Vector3d &CoM)
{
  Eigen::Vector3d normal;
  floor_projection(CoM - contact, down, normal);
  return (query - contact).dot(normal);
}


void build_carved(
  const std::vector<int>::iterator &begin,
  const std::vector<int>::iterator &end,
  const long size,
  int mask[])
{
  std::fill_n(mask, size, 1);

  for (std::vector<int>::iterator i = begin; i < end; ++i) {
    mask[*i] = -1;
  }
}


void floor_projection(
  const Eigen::Vector3d & v,
  const Eigen::Vector3d & down,
  Eigen::Vector3d & proj)
{
  proj = v - (v.dot(down) / down.squaredNorm()) * down;
}
