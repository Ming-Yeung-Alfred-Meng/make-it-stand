#ifndef MAKE_IT_STAND_HELPER_G_H
#define MAKE_IT_STAND_HELPER_G_H
#include <Eigen/Core>


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
  Eigen::Vector3d &g);
#endif //MAKE_IT_STAND_HELPER_G_H
