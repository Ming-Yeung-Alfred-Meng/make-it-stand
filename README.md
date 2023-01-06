# An Implementation of Make It Stand (add reference to the paper here)

## General Idea of the Algorithm

The algorithm shifts the center of mass of the object a given triangle mesh represents, so that it balances without easily toppling after fabrication through 3D printing. It uses two methods: 1) inner carving and 2) deformation. Inner carving creates a mesh to represent an empty region to be constructed inside the input mesh. From now on, we shall call them the inner and outer mesh, respectively. Essentailly, inner carving shift the center of mass by redistributing mass. Deformation shifts it by deforming the inner and outer mesh via linear blend skinning using bounded biharmonic weights.

## Assumption of the Algorithm

1. The object has uniform densities.

## My Implementation

### Inner Carving

#### Inputs
1. The outer mesh.
2. Contact point between the outer mesh and the ground, on which the object should balances itself.

#### Basic Settings
1. Direction of gravitational pull. In my case, it is the negative y-direction, as libigl's viewer uses the convention that the positive x- and y-direction are right and up, respectively.
2. Size of a voxel. Smaller meshes require smaller voxels, and smaller voxels enable higher precision.
3. Minimum number of voxels to carve in each iteration of inner carving
4. Minimum thickness of the object's physical shell, i.e. the minimum distance between the outer mesh and the inner mesh. This is currently calculated as the minimum distance between the outer mesh and the voxel **CENTERS**.
5. Density of the 3D printing material.

#### Steps
1. 


### Deformation

### My Implementation v.s. The Authors' Impelementation

### Known Limitations of My Implementation
1. The ideal way of measuring the minimum distance between the outer mesh and the inner mesh would be to measure from the outer mesh to the corners of the voxels.
2. Density seems to be unnecessary.

## A small showcase of progress
The followings are the outer mesh (left) and the inner void (right) after running the inner carving algorithm. The contact point between the figure and the ground is selected so that it can balance on its right foot.

![](images/outer_mesh.png)![](images/inner_mesh.png)

## What is done

1. Inner carving: The iterative processing of finding the optimal inner void to empty, and the construction of its mesh have been implemented.
  
2. Matrices for deformation: The optimal deformation is obtained via gradient descent. Functions that build the matrices required for the computation have been implemented.

3. Added GoogleTest Framework.

## What is in progress
  
1. ~~Add GoogleTest framework: To conduce an easier and more complete debugging process.~~

2. Bounded biharmonic weights(BBW) for voxel grid: BBW need to be computed for each voxel of a grid, from which meshes obtain their weights via interpolation for deformation.

3. ~~Add test suites.~~

Thanks.
