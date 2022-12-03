#include "linear_index.h"


Eigen::Index linear_index(
  const Eigen::RowVector3i &side,
  const int i,
  const int j,
  const int k)
{
  assert(0 <= i && i < side(0));
  assert(0 <= j && j < side(1));
  assert(0 <= k && k < side(2));

  return i + j * side(0) + k * side(0) * side(1);
}