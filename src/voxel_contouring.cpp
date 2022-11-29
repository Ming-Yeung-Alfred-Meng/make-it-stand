#include <unordered_map>
#include "voxel_contouring.h"
#include "linear_index.h"
#include <igl/dual_contouring.h>

void single_direction_quads(
  const char d,
  const Eigen::MatrixXd &grid,
  const Eigen::RowVector3i &side,
  const int in_out[],
  std::unordered_map<std::tuple<int, int, int>, Eigen::Index, igl::Hash> &gridV2V,
  Eigen::MatrixXd &quad_V,
  Eigen::MatrixXi &quad_F,
  Eigen::Index &num_V,
  Eigen::Index &num_F);
void quad2triF(
  const Eigen::MatrixXi &quad_F,
  const Eigen::Index &num_F,
  const int orientation,
  Eigen::MatrixXi &F);



// Is there enough padding in grid? Should I worry about this?
void voxel_contouring(
  const Eigen::MatrixXd &grid,
  const Eigen::RowVector3i &side, // is it possible to change this to 3-tuple?
  const int in_out[],
  Eigen::MatrixXd &V,
  Eigen::MatrixXi &F)
{
  std::unordered_map< std::tuple<int,int,int>, Eigen::Index, igl::Hash > gridV2V;
//  Eigen::Matrix<double, Eigen::Dynamic, 3> quad_V;
  Eigen::Index num_V = 0;
  Eigen::MatrixXi quad_F;
  Eigen::Index num_F = 0;
  int u;

  single_direction_quads(0, grid, side, in_out, gridV2V, V, quad_F, num_V, num_F);
  single_direction_quads(1, grid, side, in_out, gridV2V, V, quad_F, num_V, num_F);
  single_direction_quads(2, grid, side, in_out, gridV2V, V, quad_F, num_V, num_F);

  V.conservativeResize(num_V, V.cols());
  quad2triF(quad_F, num_F, 1, F);
  // Truncate quad_V into V using block operations and turn quad_F into F (with a helper function).
}


void single_direction_quads(
  const char d,
  const Eigen::MatrixXd &grid,
  const Eigen::RowVector3i &side,
  const int in_out[],
  std::unordered_map< std::tuple<int,int,int>, Eigen::Index, igl::Hash > &gridV2V,
  Eigen::MatrixXd &quad_V,
  Eigen::MatrixXi &quad_F,
  Eigen::Index &num_V,
  Eigen::Index &num_F)
{
  assert(d == 0 || d == 1 || d == 2);

  Eigen::Index curr_index;
  Eigen::Index next_index;
  double half_step = (grid(1, 0) - grid(0, 0)) / 2;
  int u;

  quad_V.resize(0, 3);
  quad_F.resize(0, 4);

  for (int i = 0; i < side(0) - (1 * (d == 0)); ++i) {
    for (int j = 0; j < side(1) - (1 * (d == 1)); ++j) {
      for (int k = 0; k < side(2) - (1 * (d == 2)); ++k) {
        curr_index = linear_index(side, i, j, k);
        next_index = linear_index(side, i + (1 * (d == 0)), j + (1 * (d == 1)), k + (1 * (d == 2)));
        if (in_out[curr_index] * in_out[next_index] < 0) {
          u = 0;
          for (int a = 0; a < 2; ++a) {
            for (int b = 0; b < 2; ++b) {
              std::tuple<int, int, int> key_temp;
              if (d == 0) {
                if (in_out[curr_index] > 0) {
                  key_temp = {i + 1, j + a, k + b};
                } else {
                  key_temp = {i + 1, j + b, k + a};
                }
              } else if (d == 1) {
                if (in_out[curr_index] > 0) {
                  key_temp = {i + b, j + 1, k + a};
                } else {
                  key_temp = {i + a, j + 1, k + b};
                }
              } else {
                if (in_out[curr_index] > 0) {
                  key_temp = {i + a, j + b, k + 1};
                } else {
                  key_temp = {i + b, j + a, k + 1};
                }
              }

              const std::tuple<int, int, int> key = std::move(key_temp);
              const auto iterator = gridV2V.find(key);
              if (iterator != gridV2V.end()) {
                if (num_F + 1 >= quad_F.rows()) { quad_F.conservativeResize(2 * num_F + 1, quad_F.cols()); }
                quad_F(num_F, u) = iterator->second;
              } else {
                if (num_V + 1 >= quad_V.rows()) { quad_V.conservativeResize(2 * num_V + 1, quad_V.cols()); }
                quad_V.row(num_V) = grid.row(linear_index(side, std::get<0>(key), std::get<1>(key), std::get<2>(key)));
                quad_V(num_V, 0) -= half_step;
                quad_V(num_V, 1) -= half_step;
                quad_V(num_V, 2) -= half_step;
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
}

// orientation = 0 is outward normals
void quad2triF(
  const Eigen::MatrixXi &quad_F,
  const Eigen::Index &num_F,
  const int orientation,
  Eigen::MatrixXi &F)
{
  // 1:57
  // loop over each row/quad face in quad_F, bounded by num_F (max index)
  // for each, add the oriented face indices to F
  F.resize(2 * num_F, 3);

  Eigen::Index order1[3];
  Eigen::Index order2[3];

  if (orientation == 0) {
    order1[0] = 1;
    order1[1] = 2;
    order1[2] = 3;
    order2[0] = 2;
    order2[1] = 4;
    order2[2] = 3;
  } else {
    order1[0] = 1;
    order1[1] = 3;
    order1[2] = 2;
    order2[0] = 2;
    order2[1] = 3;
    order2[2] = 4;
  }

  for (int i = 0; i < num_F; ++i) {
      F.row(2 * i) << quad_F(i, order1[0]), quad_F(i, order1[1]), quad_F(i, order1[2]);
      F.row(2 * i + 1) << quad_F(i, order2[0]), quad_F(i, order2[1]), quad_F(i, order2[2]);
  }
}
