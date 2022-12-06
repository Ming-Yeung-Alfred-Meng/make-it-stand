I aimed to implement the paper "Make It Stand: Balancing Shapes for 3D Fabrication."

The goal of the algorithm is to shift the center of mass of the mesh so that it balances after fabrication. The algorithm has two parts: 1) inner carving & 2) deformation. Inner carving aims to shift the center of mass by creating a void region within the original mesh. It outputs an inner mesh to represent the boundary of the inner void. Deformation simply shift the center of mass via linear blend skinning deformation model.

I apologize that I have only implemented the inner carving part, and it is still a little buggy. I have implemented most of the functions necessary to make the deformation part running.

.cpp files concerned with inner carving specifically are: 

inner_carving.cpp: the implementation of the inner carving part.

voxel_contouring.cpp: function that construct the inner mesh.

other helper functions: cube.cpp, face_contribution_to_com.cpp, linear_index.cpp,  and voxel_indices.cpp

.cpp files concerned with deformation specifically are: 

deformation.cpp: the implementation of the deformation part.

laplacian.cpp, mass_grad.cpp, center_of_mass_grad.cpp, my_lbs_matrix.cpp: functions that build the matrices required to run the gradient descent optimization described in section 5.2 of the paper.

.cpp files concerned with both inner carving and deformation: center_of_mass.cpp, helper_g.cpp.

To see the algorithm creating an inner mesh given descimated-knight.off as input, please watch my video located at the root direction. I think the output is incorrect, I made the knight to balance on its right foot, so I expected the left side of the knight to be mostly empty, and the right side to be mostly filled, but the output had part of both the left and right side carved. I think the bug arises because the code incorrectly calculates the order of voxels carve.

My name: Ming Yeung Alfred Meng
Paper: Make It Stand: Balancing Shapes for 3D Fabrication
video: https://www.youtube.com/watch?v=9ByYrmvihog

Thanks for reading.