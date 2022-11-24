#include "voxel_contouring.h"


// Is there enough padding in grid? Should I worry about this?
void voxel_contouring(
  const Eigen::MatrixXd &grid,
  const Eigen::RowVector3i &side, // is it possible to change this to 3-tuple?
  const int in_out[],
  Eigen::MatrixXd &V,
  Eigen::MatrixXd &F)
{
  // create an unordered_map: tuple<int, int, int> to eigen::index
  // go over each dimension by index, for each, go from bottom to top
  // e.g. for each i,j combination, loop k from 0 to max. do the same for (i, k) and (j, k)
  int prev = 1;
  for (int i = 0; i < side(0); ++i) {
    for (int j = 0; j < side(1); ++j) {
      for (int k = 0; k < side(2); ++k) {

      }
    }
  }
}