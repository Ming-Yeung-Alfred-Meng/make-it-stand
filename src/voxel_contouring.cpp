#include <unordered_map>
#include "voxel_contouring.h"
#include "linear_index.h"
#include <igl/dual_contouring.h>


void single_direction_quads(
  const int d,
  const Eigen::MatrixXd &grid,
  const Eigen::RowVector3i &side,
  const int in_out[],
  std::unordered_map< std::tuple<int,int,int>, Eigen::MatrixXd::Index, igl::Hash > &gridV2V,
  Eigen::Matrix<double, Eigen::Dynamic, 3> &quad_V,
  Eigen::Matrix<long, Eigen::Dynamic, 4> &quad_F,
  Eigen::Index &num_V,
  Eigen::Index &num_F)
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
  Eigen::MatrixXd::Index curr_index;
  Eigen::MatrixXd::Index next_index;
  double step = grid(0, 1) - grid(0, 0);
  int u;

  for (int i = 0; i < side((d + 1) % 3); ++i) {
    for (int j = 0; j < side((d + 2) % 3); ++j) {
      for (int k = 0; k < side(d % 3); ++k) {
        if (d == 0) {
          curr_index = linear_index(side, k, i, j + 1);
        } else if (d == 1) {
          curr_index = linear_index(side, j, k, i + 1);
        } else {
          curr_index = linear_index(side, i, j, k + 1);
        }
        next_index = curr_index + side((d + 1) % 3) * side((d + 2) % 3);

        if (in_out[curr_index] * in_out[next_index] < 0) {
          u = 0;
          for (int a = 0; a < 2; ++a) {
            for (int b = 0; b < 2; ++b) {
              if (d == 0) {
                curr_index = linear_index(side, k, i, j + 1);
              } else if (d == 1) {
                curr_index = linear_index(side, j, k, i + 1);
              } else {
                const std::tuple<int, int, int> key {i + a, j + b, k + 1};
              }

              const auto iterator = gridV2V.find(key);
              if (iterator != gridV2V.end()) {
                if (num_F + 1 >= quad_F.rows()) { quad_F.conservativeResize(2 * num_F + 1, quad_F.cols()); }
                quad_F(num_F, u) = iterator->second;
              } else {
                if (num_V + 1 >= quad_V.rows()) { quad_V.conservativeResize(2 * num_V + 1, quad_V.cols()); }
                quad_V.row(num_V) = grid.row(next_index);
                quad_V(num_V,0) += a - step / 2;
                quad_V(num_V,1) += b - step / 2;
                quad_V(num_V, 2) -= step / 2;
                gridV2V[key] = num_V;
                quad_F(num_F, u) = num_V;
                ++num_V;
              }
              ++u;
            }
          }
          ++num_F;
        }
      }
    }
  }
}


// Is there enough padding in grid? Should I worry about this?
void voxel_contouring(
  const Eigen::MatrixXd &grid,
  const Eigen::RowVector3i &side, // is it possible to change this to 3-tuple?
  const int in_out[],
  Eigen::MatrixXd &V,
  Eigen::MatrixXd &F)
{
  std::unordered_map< std::tuple<int,int,int>, Eigen::MatrixXd::Index, igl::Hash > gridV2V;
  Eigen::Matrix<double, Eigen::Dynamic, 3> quad_V;
  typename decltype(quad_V)::Index num_V = 0;
  Eigen::Matrix<long, Eigen::Dynamic, 4> quad_F;
  typename decltype(quad_F)::Index num_F = 0;
  Eigen::MatrixXd::Index next_index;
  int u;
  double step = grid(0, 1) - grid(0, 0);

  single_direction_quads(0, grid, side, in_out, gridV2V, quad_V, quad_F, num_V, num_F);
  single_direction_quads(1, grid, side, in_out, gridV2V, quad_V, quad_F, num_V, num_F);
  single_direction_quads(2, grid, side, in_out, gridV2V, quad_V, quad_F, num_V, num_F);
}