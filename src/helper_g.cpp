#include "helper_g.h"


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