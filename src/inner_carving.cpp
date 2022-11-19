#include "inner_carving.h"


double carving_energy()
{
  return -1;
}


double update_carving_energy()
{
  return -1;
}


void inner_carving()
{
  // initialize an empty voxel grid
  //    using igl::voxel_grid.h
  // initialize center of mass
  //    using center_of_mass for outer mesh only version
  // initialize energy
  // run (first) iteration:
  //    initialize priority queue
  //      rank from largest to smallest distances
  //    insert filled voxel centers
  //      priority of voxel centers calculated according to current plane
  //    extract and carve voxels one by one (second iteration),
  //    update new center of mass
  //    calculate new energy
  //    keep track of min energy seen so far
  //    keep track of the iteration where min energy occurred
  //    end carving (second iteration) when all voxels with positive distance are evaluated
  //
}