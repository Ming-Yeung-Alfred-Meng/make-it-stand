# An Implementation of Make It Stand

I plan to complete this project by January 9th 2023.

## A small showcase of progress:

![](images/inner_mesh.png)

## What is done:

1. Inner carving: The iterative processing of finding the optimal inner void to carve, and the construction of its mesh have been implemented.
  
2. Matrices for deformation: The optimal deformation is obtained via gradient descent. Functions that build the matrices required for the computation have been implemented.

## What is under progress:
  
1. Add googletest framework: To aid an easier and more complete debugging process.

2. Bounded biharmonic weights(BBW) for voxel grid: BBW need to be computed for each voxel of a grid, from which meshes obtain their weights via interpolation for deformation.
  
Thanks.
