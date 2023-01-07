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
1. Use igl::voxel_grid() to construct a voxel grid surrounding the outer mesh. Include a padding of at least one voxel, as it simplifies the later step of constructing the inner mesh. I used the overload of igl::voxel_grid() that takes the vertices of the outer mesh as an input. It fixes the padding count to be 1, so setting the bounding box offset to be 0 creates an adequate voxel grid.

2. Compute the center of mass and mass of the object using igl::centroid()'s outputs of centroid and volume, as the assumption that input objects have uniform density ensures the equality of centroid and center of mass, and the density is available.
 
3. Compute the initial value of the energy concerned with inner carving, which we shall call the carving energy $C$, and it is in general the squared norm of the horizonal component of the vector $d = \text{center of mass} - \text{contact}$:

$$C = ||d - \frac{d \cdot g}{||g||^2}g||^2$$

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Where $g$ is the direction of gravitational pull.

4. Iteratively identify indicies of voxels in the voxel grid that are within the outer mesh by at least the minimum thickness. igl::signed_distance() can use fast winding number to check if a point is inside a mesh and calculate its distance from it. We only consider voxel centers that has a distance $s$ outputed by igl::signed_distance() s.t. $s \leq 0$ and $|s| \geq \text{minimum thickness}$.

5. Imagine a plane that intersects the contact point $c$ and is perpendicular to $d$. It cuts the object into two regions. We now sort all voxel centers $r$ (survived from step 4) in decreasing order of their signed distance $s$ from the plane, which is computed as:

$$s = (r - c) \cdot (d - \frac{d \cdot g}{||g||^2}g)$$

Those that are in the same region as the center of mass have positive signed distance. There is no need to ever compute the plane. 

Concretely, use std::sort() and a custom comparator that is a lambda function that takes two voxel indices as inputs, and compute and compare their signed distance from the imaginary plane. 

6. The main loop of inner carving. Loop through the sorted voxels that have non-negative signed distance, and "carve" them one by one, i.e. update the center of mass and mass of the object at each iteration. Let $\rho$ be the density, and $CoM_old$ and $m_old$ be the center of mass and mass before "carving" the current voxel in consideraion, respectively, their counterparts after "carving", $CoM_{new}$ and $m_{new}$ are:

$$CoM_{new} = \frac{CoM_{old} \cdot m_{old}}{m_{new}} - \frac{\rho}{24m_{new}}\sum_{f \in F} ((v_j - v_i) \times (v_k - v_i)) * g(v_i, v_j, v_k)$$

$$m_{new} = m_{old} - \rho \cdot l^3$$

Find the optimal sequence of voxels
Construct inner mesh

### Deformation

### My Implementation v.s. The Authors' Impelementation
1. The authors' implementation has two balancing mode: 1) the standing mode and 2) the suspension mode, while mine only implemented the standing mode.
2. The authors' implementation does not sort voxels that have negative signed distance.

### Known Limitations of My Implementation
1. The ideal way of measuring the minimum distance between the outer mesh and the inner mesh would be to measure from the outer mesh to the corners of the voxels.
2. Density seems to be unnecessary.
3. Storing the voxel grid as a matrix of voxel centers may not be the best way to do so.
4. Deformation has yet to be completed, as calculating bounded biharmonic weights for the voxel grid has yet to be compeleted.
5. The authors suggest that there should be no floating components, but my implementation results in them.

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
