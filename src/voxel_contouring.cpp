#include <unordered_map>
#include "voxel_contouring.h"
#include "linear_index.h"
#include <igl/dual_contouring.h>


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
  // check if the next sign is different from the current cell
  // if so, fetch the index (wrt the grid) of the four corners of the face,
  // check if the indices are in the map
  // if in the map, add the indices wrt the output V into F
  // if not in the map, insert the vertex into V, add the index pair into the map, and add index wrt V into F
  // if sign are the same, ignore.
  // process F, into triangular faces.
  std::unordered_map< std::tuple<int,int,int>, Eigen::MatrixXd::Index, igl::Hash > gridV2V;
  std::vector<Eigen::Vector4i, Eigen::aligned_allocator<Eigen::Vector4i>> F_temp;
  Eigen::Vector4i corners;
  Eigen::RowVector3d corner;
  Eigen::MatrixXd::Index next_index;
  int u;
  double step = grid(0, 1) - grid(0, 0);

  for (int i = 0; i < side(0); ++i) {
    for (int j = 0; j < side(1); ++j) {
      for (int k = 0; k < side(2); ++k) {
        next_index = linear_index(side, i, j, k + 1);
        if (in_out[linear_index(side, i, j, k)] * in_out[next_index] < 0) {
          u = 0;
          for (int a = 0; a < 2; ++a) {
            for (int b = 0; b < 2; ++b) {
              const auto iterator = gridV2V.find({i + a, j + b, k + 1});
              if (iterator != gridV2V.end()) {
                corners(u) = iterator->second;
              } else {
                corner = grid.row(next_index);
                corner(2) -= step / 2;
                corner(0) += a - step / 2;
                corner(1) += b - step / 2;

              }

              ++u;
            }
          }
          F_temp.push_back(corners); // do I need to make a copy here?
        }
      }
    }
  }
}