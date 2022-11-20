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
  //                  initialize priority queue
  //                  rank from largest to smallest distances
  //                  insert filled voxel centers
  //                      priority of voxel centers calculated according to current plane
  // initialize an array of indices of grid point that are within a distance from hull
  // sort the array using custom comparator
  // run (first) iteration:
  //    make a copy of the energy
  //    extract indices of filled voxels from the sorted array one by one (second iteration),
  //                  insert voxel center into a queue
  //      fetch corner using the index
  //      update new center of mass
  //      calculate new energy
  //      keep track of min energy seen so far
  //      keep track of the iteration where min energy occurred
  //    end carving (second iteration) when all voxels with positive distance are evaluated
  //    if new energy is greater or equal to the copy of energy or the number of voxels carved is too few,
  //      exit (first iteration)
  //    else,
  //                        initialize a new
  //                          extract all voxels that remained filled into a new priority queue
  //      sort the remaining indices in the array by new comparator based on new center of mass
  //    currently it is unsure if returning the optimized energy is necessary
  // use the iteration number where the optimized energy occurred to locate the last carved voxel
  // use the index and array of index to construct the -1, 0, 1 inside-outside grid vector
  // use dual_contour to construct inner mesh
}