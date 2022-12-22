#include <igl/min_quad_with_fixed.h>
#include <igl/read_triangle_mesh.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/project.h>
#include <igl/unproject.h>
#include <igl/snap_points.h>
#include <igl/unproject_onto_mesh.h>
#include <Eigen/Core>
#include <iostream>
#include <stack>
#include "arap_precompute.h"
#include "arap_single_iteration.h"
#include "biharmonic_precompute.h"
#include "biharmonic_solve.h"
#include "inner_carving.h"
#include <igl/lbs_matrix.h>

// Undoable
struct State
{
    // Rest and transformed control points
    Eigen::MatrixXd CV, CU;
    Eigen::Index contact = -1;
    // 0: placing handle
    // 1: choosing contact point
    // 2: user deformation
    // 3: balance
    int state = 0;
} s;

int main(int argc, char *argv[])
{
  Eigen::Matrix3d m;
  m << 1, 5, 9,
       4, 2, 6,
       7, 8, 3;

  Eigen::Vector3d min = m.colwise().minCoeff();
  std::cout << "min:\n" << min << std::endl;

//  // Undo Management
//  std::stack<State> undo_stack,redo_stack;
//  const auto push_undo = [&](State & _s=s)
//  {
//      undo_stack.push(_s);
//      // clear
//      redo_stack = std::stack<State>();
//  };
//
//  const auto undo = [&]()
//  {
//      if(!undo_stack.empty())
//      {
//        redo_stack.push(s);
//        s = undo_stack.top();
//        undo_stack.pop();
//      }
//  };
//
//  const auto redo = [&]()
//  {
//      if(!redo_stack.empty())
//      {
//        undo_stack.push(s);
//        s = redo_stack.top();
//        redo_stack.pop();
//      }
//  };
//// decimated-knight: voxel_scale = 100, thickness = 0.01
//  const int voxel_scale = 100; // cannot be too small!
//  const int min_carve = 2;
//  const double thickness = 0.01;
//  const double density = 1;
//  Eigen::MatrixXd MiV;
//  Eigen::MatrixXi MiF;
//  Eigen::MatrixXd V,U;
//  Eigen::MatrixXi F;
//  long sel = -1;
//  Eigen::RowVector3f last_mouse;
//  igl::min_quad_with_fixed_data<double> biharmonic_data, arap_data;
//  Eigen::SparseMatrix<double> arap_K;
//
//  // Load input meshes
//  igl::read_triangle_mesh(
//    (argc>1?argv[1]:"../data/decimated-knight.off"),V,F);
//
//  std::cerr << "V:\n" << V << std::endl;
//  //(argc > 1 ? argv[1] : "../data/knight.off"), V, F);
////    (argc > 1 ? argv[1] : "../data/bunny.off"), V, F);
//
//  U = V;
//  igl::opengl::glfw::Viewer viewer;
//  std::cout<<R"(
//[click]  To place new control point
//[drag]   To move control point
//[space]  Toggle whether placing control points or deforming
//M,m      Switch deformation methods
//U,u      Update deformation (i.e., run another iteration of solver)
//R,r      Reset control points
//⌘ Z      Undo
//⌘ ⇧ Z    Redo
//)";
//
//  enum Method
//  {
//      BIHARMONIC = 0,
//      ARAP = 1,
//      NUM_METHODS = 2,
//  } method = BIHARMONIC;
//
//  const auto & update = [&]()
//  {
//      // predefined colors
//      const Eigen::RowVector3d orange(1.0,0.7,0.2);
//      const Eigen::RowVector3d yellow(1.0,0.9,0.2);
//      const Eigen::RowVector3d blue(0.2,0.3,0.8);
//      const Eigen::RowVector3d green(0.2,0.6,0.3);
//      const Eigen::RowVector3d red(1,0.0,0.0);
//      if(s.state == 0) {
//        std::cerr << "Successfully entered handle placing state." << std::endl;
//        viewer.data().set_mesh(V,F);
//        viewer.data().show_lines = false;
//        viewer.core().is_animating = true;
//        viewer.data().face_based = true;
//
//        viewer.data().set_vertices(V);
//        viewer.data().set_colors(blue);
//        viewer.data().set_points(s.CV,orange);
//      } else if (s.state == 2 && s.CV.rows()>0 && s.contact != -1) {
//        std::cerr << "Successfully entered user deformation state." << std::endl;
//        // SOLVE FOR DEFORMATION
//        switch(method)
//        {
//          default:
//          case BIHARMONIC:
//          {
//            Eigen::MatrixXd D;
//            biharmonic_solve(biharmonic_data,s.CU-s.CV,D);
//            U = V+D;
//            break;
//          }
//          case ARAP:
//          {
//            arap_single_iteration(arap_data,arap_K,s.CU,U);
//            break;
//          }
//        }
//        viewer.data().set_vertices(U);
//        viewer.data().set_colors(method==BIHARMONIC?orange:yellow);
//        viewer.data().set_points(s.CU,method==BIHARMONIC?blue:green);
//      } else if (s.state == 3 && s.CV.rows()>0 && s.contact != -1) {
//        std::cerr << "Successfully entered balancing state." << std::endl;
//
//        inner_carving(U, F, s.CU.row(s.contact), voxel_scale, min_carve, thickness, density, MiV, MiF);
//
//        viewer.data().clear();
//
//        viewer.data().set_mesh(MiV,MiF);
//        viewer.data().show_lines = false;
//        viewer.core().is_animating = true;
//        viewer.data().face_based = true;
//
//        viewer.data().set_vertices(MiV);
//        viewer.data().set_colors(red);
//      }
//      viewer.data().compute_normals();
//  };
//
//
//  viewer.callback_mouse_down =
//    [&](igl::opengl::glfw::Viewer&, int, int)->bool
//    {
//        last_mouse = Eigen::RowVector3f(
//          viewer.current_mouse_x,viewer.core().viewport(3)-viewer.current_mouse_y,0);
//        if(s.state == 0) {
//          // Find closest point on mesh to mouse position
//          int fid;
//          Eigen::Vector3f bary;
//          if (igl::unproject_onto_mesh(
//            last_mouse.head(2),
//            viewer.core().view,
//            viewer.core().proj,
//            viewer.core().viewport,
//            V, F,
//            fid, bary)) {
//            long c;
//            bary.maxCoeff(&c);
//            Eigen::RowVector3d new_c = V.row(F(fid, c));
//            if (s.CV.size() == 0 || (s.CV.rowwise() - new_c).rowwise().norm().minCoeff() > 0) {
//              push_undo();
//              s.CV.conservativeResize(s.CV.rows() + 1, 3);
//              // Snap to closest vertex on hit face
//              s.CV.row(s.CV.rows() - 1) = new_c;
//              update();
//              return true;
//            }
//          }
//        } else {
//          Eigen::MatrixXf CP;
//          igl::project(
//            Eigen::MatrixXf(s.CU.cast<float>()),
//            viewer.core().view,
//            viewer.core().proj, viewer.core().viewport, CP);
//          Eigen::VectorXf D = (CP.rowwise() - last_mouse).rowwise().norm();
//          sel = (D.minCoeff(&sel) < 30) ? sel : -1;
//          if (sel != -1) {
//            if (s.state == 1) {
//              s.contact = sel;
//              std::cout << "A contact point has been selected." << std::endl;
//            } else {
//              last_mouse(2) = CP(sel, 2);
//            }
//            push_undo();
//            update();
//            return true;
//          }
//        }
//        return false;
//    };
//
//
//  viewer.callback_mouse_move = [&](igl::opengl::glfw::Viewer &, int,int)->bool
//  {
//      if(sel!=-1)
//      {
//        Eigen::RowVector3f drag_mouse(
//          viewer.current_mouse_x,
//          viewer.core().viewport(3) - viewer.current_mouse_y,
//          last_mouse(2));
//        Eigen::RowVector3f drag_scene,last_scene;
//        igl::unproject(
//          drag_mouse,
//          viewer.core().view,
//          viewer.core().proj,
//          viewer.core().viewport,
//          drag_scene);
//        igl::unproject(
//          last_mouse,
//          viewer.core().view,
//          viewer.core().proj,
//          viewer.core().viewport,
//          last_scene);
//        s.CU.row(sel) += (drag_scene-last_scene).cast<double>();
//        last_mouse = drag_mouse;
//        update();
//        return true;
//      }
//      return false;
//  };
//
//
//  viewer.callback_mouse_up = [&](igl::opengl::glfw::Viewer&, int, int)->bool
//  {
//      sel = -1;
//      return false;
//  };
//
//
//
//  viewer.callback_key_pressed =
//    [&](igl::opengl::glfw::Viewer &, unsigned int key, int mod)
//    {
//        switch(key)
//        {
//          case 'M':
//          case 'm':
//          {
//            method = (Method)(((int)(method)+1)%((int)(NUM_METHODS)));
//            break;
//          }
//          case 'R':
//          case 'r':
//          {
//            push_undo();
//            s.CU = s.CV;
//            break;
//          }
//          case 'U':
//          case 'u':
//          {
//            // Just trigger an update
//            break;
//          }
//          case ' ':
//            push_undo();
//            s.state = (s.state + 1) % 4;
//            std::cerr << "State = " << s.state << std::endl;
//            if (s.state == 1 && s.CV.rows()>0) {
//              s.CU = s.CV;
//            }
//
//            if (s.state == 2 && s.CV.rows()>0 && s.contact != -1)
//            {
//              // Switching to deformation mode
//              Eigen::VectorXi b;
//              igl::snap_points(s.CV,V,b);
//              // PRECOMPUTATION FOR DEFORMATION
//              biharmonic_precompute(V,F,b,biharmonic_data);
//              arap_precompute(V,F,b,arap_data,arap_K);
//            }
//            break;
//          default:
//            return false;
//        }
//        update();
//        return true;
//    };
//
//  // Special callback for handling undo
//  viewer.callback_key_down =
//    [&](igl::opengl::glfw::Viewer &, unsigned char key, int mod)->bool
//    {
//        if(key == 'Z' && (mod & GLFW_MOD_SUPER))
//        {
//          (mod & GLFW_MOD_SHIFT) ? redo() : undo();
//          update();
//          return true;
//        }
//        return false;
//    };
//  viewer.callback_pre_draw =
//    [&](igl::opengl::glfw::Viewer &)->bool
//    {
//        if(viewer.core().is_animating && s.state == 2 && method == ARAP)
//        {
//          arap_single_iteration(arap_data,arap_K,s.CU,U);
//          update();
//        }
//        return false;
//    };
//
//  viewer.data().set_mesh(V,F);
//  viewer.data().show_lines = false;
//  viewer.core().is_animating = true;
//  viewer.data().face_based = true;
//  update();
//  viewer.launch();
//  return EXIT_SUCCESS;
}
