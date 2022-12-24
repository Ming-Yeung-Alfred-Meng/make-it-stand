#ifndef CURVATURE_INNER_CARVING_H
#define CURVATURE_INNER_CARVING_H
#include <Eigen/Core>


// Performs the inner carving from section 4 of
// "Make It Stand: Balancing Shapes for 3D Fabrication" [Prevost et al. 2013].
//
// Inputs:
//   MoV  #MoV by 3 list of vertices of the outer mesh
//   MoF  #MoF by 3 list of indices into MoV
//   contact  3D contact location between the object and the ground (assumed to be the xy-plane)
//   voxel_scale  number of voxels along the longest side of the voxel grid
//   min_carve  minimum number of voxels to carve in each iteration of the algorithm
//   thickness  minimum distance between the inner and outer mesh
//   density  density of the object
//
// Outputs:
//   MiV  #MiV by 3 list of vertices of the inner mesh representing the boundary of the carved region
//   MiF  #MiF by 3 list of indices into MiV
//
void inner_carving(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::Vector3d &down,
  const Eigen::Vector3d &contact,
  int voxel_scale,
  int min_carve,
  double thickness,
  double density,
  Eigen::MatrixXd &MiV,
  Eigen::MatrixXi &MiF);

// Helper functions:

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
//
double carving_energy(const Eigen::Vector3d &down, const Eigen::Vector3d &contact, const Eigen::Vector3d &CoM);


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
//
void update_center_of_mass(const Eigen::Vector3d &center, double length, double old_mass, double new_mass, double density, Eigen::Vector3d &CoM);


// Compute new mass of the object after carving a voxel.
//
// Inputs:
//   old_mass  old mass of the object
//   density  density of the object
//   length  length of a side of the (regular) voxel
//
// Return:
//   new mass of the object carving a voxel.
//
double reduce_mass_by_a_voxel(double old_mass, double density, double length);


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
void build_carved(const std::vector<int>::iterator &begin, const std::vector<int>::iterator &end, long size, int mask[]);


// Build an array of binary mask indicating which voxels are
// carved.
//
// Inputs:
//   begin  start of the vector containing indices of carved voxels.
//   end  std::end() of the vector
//   size  size of the vector
// Output:
//   mask  binary mask s.t. mask[i] = 1 if the ith voxel is not carved
//                          mask[i] = -1 if it is carved.
//
double distance_from_plane(const Eigen::Vector3d &query, const Eigen::Vector3d &down, const Eigen::Vector3d &contact, const Eigen::Vector3d &CoM);

void floor_projection(const Eigen::Vector3d & v, const Eigen::Vector3d & down, Eigen::Vector3d & proj);
#endif //CURVATURE_INNER_CARVING_H
