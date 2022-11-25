#include <igl/avg_edge_length.h>
#include <igl/read_triangle_mesh.h>
#include <igl/parula.h>
#include <igl/doublearea.h>
#include <igl/opengl/glfw/Viewer.h>
#include <Eigen/Core>
#include <igl/winding_number.h>
#include <igl/grid.h>
#include <igl/voxel_grid.h>
#include <cmath>


int main(int argc, char *argv[])
{
  int a[3];
  a = {1, 2, 3};
  std::cout << "a: " << a << std::endl;

//  char a = 1;
//  std::cout << (a == 2) << std::endl;
//  Eigen::MatrixXd m;
//  std::cout << "m.rows() & m.cols(): " << m.rows() << " & " << m.cols() << std::endl;
//  Eigen::RowVector3d r(1., 2., 3.);
//  Eigen::MatrixXd m(3, 3);
//  m << r;

//  std::cout << "m:\n" << m << std::endl;
//  Eigen::Matrix2Xd m;
//  std::cout << "m.rows():\n" << m.rows() << std::endl;
//  std::vector<int> x = {1, 2, 3};
//  Eigen::MatrixXd::Index i;
//
//  std::cout << "i: " << i << std::endl;
//  i = 3 + 1;
//  std::cout << "i: " << i << std::endl;
//  i = 9;
//  std::cout << "i: " << i << std::endl;
//
//  i = 1;
//
//  std::cout << "x[1]: " << x[i] << std::endl;

//  std::vector<int> l = {1, 3, 2};
//
//  std::sort(l.begin() + 2, l.end());
//
//  for (int i : l) {
//    std::cout << i << ' ';
//  }
//
//  std::cout << std::endl;
//
//  std::sort(l.begin() + 7, l.end());
//
//  for (int i : l) {
//    std::cout << i << ' ';
//  }
//
//  std::cout << std::endl;
//
//  std::sort(l.begin() + 1, l.end());
//
//  for (int i : l) {
//    std::cout << i << ' ';
//  }

//  std::cout << std::abs(-0.33) << std::endl;

//  Eigen::RowVector3d v(1, 2, 3);
//
//  std::cout << "v.rows(): " << v.rows() << std::endl;
//  std::cout << "v.cols(): " << v.cols() << std::endl;
//  std::cout << "v.row(0): " << v.row(0) << std::endl;
//
//  Eigen::MatrixXd m(2, 3);
//  m << 1, 1, 1, 2, 2, 2;
//
//  std::cout << "m.row(0).rows(): " << m.row(0).rows() << std::endl;
//  std::cout << "m.row(0).cols(): " << m.row(0).cols() << std::endl;
//  std::cout << "v.row(0).row(0): " << v.row(0).row(0) << std::endl;
//  std::cout << "m(3): " << m(3) << std::endl;
//
//  Eigen::VectorXd a;
//  a.resize(10, 1);
//
//  std::cout << "a:\n" << a << std::endl;
//
//  double s = 1.-2.*2;
//
//  Eigen::MatrixXd V(8, 3);
//  V << 0, 0, 0,
//       1, 0, 0,
//       0, 1, 0,
//       1, 1, 0,
//       0, 0, 1,
//       1, 0, 1,
//       0, 1, 1,
//       1, 1, 1;
//
//  Eigen::MatrixXd grid;
//  Eigen::RowVector3i side;
//  igl::voxel_grid(V, 0, 10, 1, grid, side);
//  std::cout << "grid:\n" << grid << std::endl;
//  std::cout << "side:\n" << side << std::endl;
//  Eigen::MatrixXd G;
//  Eigen::Vector3i size(3, 3, 3);
//
//  igl::grid(size, G);
//
//  std::cout << "G:\n" << G << std::endl;

//  Eigen::MatrixXd V(4, 3);
//  V << 0, 0, 0,
//       1, 0, 0,
//       0, 1, 0,
//       1, 0, 1;
//
//  Eigen::MatrixXi F(4, 3);
//  F << 0, 1, 2,
//       0, 2, 3,
//       2, 1, 3,
//       0, 3, 1;
//
//  Eigen::MatrixXd Q(1, 3);
//  Q << 0.5, 0.5, 0.1;
//
//  Eigen::MatrixXi W;
//
//  igl::winding_number(V, F, Q, W);
//
//  std::cout << "winding numbers:\n" << W << std::endl;

//  // Scale for the color axis
//  double scale = 100.0;
//  Eigen::MatrixXd V;
//  Eigen::MatrixXi F;
//  // Load input meshes
//  igl::read_triangle_mesh(
//    (argc>1?argv[1]:"../data/cactus.obj"),V,F);
//
//  Eigen::SparseMatrix<double> M;
//  igl::massmatrix(V,F,igl::MASSMATRIX_TYPE_DEFAULT,M);
//  Eigen::VectorXd A = M.diagonal();
//
//  Eigen::VectorXd D,G,H,K1,K2;
//  Eigen::MatrixXd D1,D2;
//  // Angle defect ~ locally integrated Gaussian curvature
//  angle_defect(V,F,D);
//  // average locally (i.e., "un-integrate" to pointwise quantity for
//  // visualization)
//  G = D.array()/A.array();
//  mean_curvature(V,F,H);
//  principal_curvatures(V,F,D1,D2,K1,K2);
//
//  igl::opengl::glfw::Viewer viewer;
//  std::cout<<R"(
//S,s      Stretch, squish color axis range
//G        Show Gaussian curvature (using principal_curvatures)
//g        Show Gaussian curvature (using angle_defect)
//M        Show discrete mean curvature (using principal_curvatures)
//m        Show discrete mean curvature (using mean_curvature)
//K        Show maximum curvature (using principal_curvatures)
//k        Show minimum curvature (using principal_curvatures)
//D,d      Show principal directions
//)";
//  // Default to mean curvature
//  Eigen::VectorXd Z = H;
//  const auto update = [&]()
//  {
//    Eigen::MatrixXd C;
//    igl::parula(Z,-scale,scale,C);
//    viewer.data().set_colors(C);
//  };
//  viewer.callback_key_pressed =
//    [&](igl::opengl::glfw::Viewer &, unsigned int key, int mod)
//  {
//    switch(key)
//    {
//      case 'D':
//      case 'd':
//        viewer.data().show_overlay ^= 1;
//        break;
//      case 'G':
//        Z = K1.array()*K2.array();
//        break;
//      case 'g':
//        Z = G;
//        break;
//      case 'K':
//        Z = K1;
//        break;
//      case 'k':
//        Z = K2;
//        break;
//      case 'M':
//        Z = 0.5*(K1+K2);
//        break;
//      case 'm':
//        Z = H;
//        break;
//      case 'S':
//      case 's':
//        scale *= key=='S' ? 2.0 : 0.5;
//        std::cout<<"Color axis range: ["<<-scale<<","<<scale<<"]"<<std::endl;
//        break;
//      default:
//        return false;
//    }
//    update();
//    return true;
//  };
//
//  viewer.data().set_mesh(V,F);
//  Eigen::MatrixXd lP(V.rows()*4,3);
//  const double h = igl::avg_edge_length(V,F);
//  lP << V-0.5*h*D1, V+0.5*h*D1, V-0.5*h*D2, V+0.5*h*D2;
//  Eigen::MatrixXi lE(2*V.rows(),2);
//  Eigen::MatrixXd lC(2*V.rows(),3);
//
//  const Eigen::RowVector3d orange(1.0,0.7,0.2);
//  const Eigen::RowVector3d yellow(1.0,0.9,0.2);
//  const Eigen::RowVector3d blue(0.2,0.3,0.8);
//  for(int e = 0;e<V.rows();e++)
//  {
//    lE(e,0)          = e+0*V.rows();
//    lE(e,1)          = e+1*V.rows();
//    lE(V.rows()+e,0) = e+2*V.rows();
//    lE(V.rows()+e,1) = e+3*V.rows();
//    lC.row(         e) = orange;
//    lC.row(V.rows()+e) = blue;
//  }
//  viewer.data().set_edges(lP,lE,lC);
//
//  update();
//  viewer.data().show_lines = false;
//  viewer.data().show_overlay = false;
//  viewer.data().face_based = false;
//  viewer.launch();
//  return EXIT_SUCCESS;
}
