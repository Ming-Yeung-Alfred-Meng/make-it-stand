#ifndef CURVATURE_INNER_CARVING_H
#define CURVATURE_INNER_CARVING_H
#include <Eigen/Core>

void inner_carving(
  const Eigen::MatrixXd &MoV,
  const Eigen::MatrixXi &MoF,
  const Eigen::Vector3d &contact,
  int voxel_scale,
  int min_carve,
  double thickness,
  double density,
  Eigen::MatrixXd &MiV,
  Eigen::MatrixXi &MiF);

double carving_energy(const Eigen::Vector3d &CoM, const Eigen::Vector3d &contact);
void update_center_of_mass(const Eigen::Vector3d &center, double length, double old_mass, double new_mass, double density, Eigen::Vector3d &CoM);
double reduce_mass_by_a_voxel(double old_mass, double density, double length);
void build_in_out(const std::vector<int>::iterator &begin, const std::vector<int>::iterator &end, long size, int in_out[]);
std::function<bool (int, int)> generate_comp(const Eigen::MatrixXd &grid, const Eigen::Vector3d &contact, const Eigen::Vector3d &CoM);
double distance_from_plane(const Eigen::Vector3d &query, const Eigen::Vector3d &contact, const Eigen::Vector3d &CoM);
#endif //CURVATURE_INNER_CARVING_H
