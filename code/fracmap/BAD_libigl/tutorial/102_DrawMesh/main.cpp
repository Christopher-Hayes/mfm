#include "tutorial_shared_path.h"
#include <igl/get_seconds.h>
#include <igl/material_colors.h>
// Eigen
#include <Eigen/Core>
// Viewer
#include <igl/opengl/glfw/Viewer.h>
// #include <imgui/imgui.h>
// IMGUI
// #include <igl/opengl/glfw/imgui/ImGuiMenu.h>
// #include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
// #include "imgui/imgui.h"
// #include "imgui_helper/ImGuiMenu.h"
// #include "imgui_helper/ImGuiHelpers.h"
// NANOGUI
#include <nanogui/nanogui.h>
#include <iostream>

using namespace nanogui;
// Read
#include <igl/readOBJ.h>
#include <igl/read_triangle_mesh.h>

#include <iostream>

// TESTING DATA

int main(int argc, char * argv[])
{
  using namespace std;
  using namespace igl;

  // Viewer
  opengl::glfw::Viewer viewer;
  // Rotation motion
  const auto & transform = [](const double t)->Eigen::Affine3d {
    Eigen::Affine3d T = Eigen::Affine3d::Identity();
    T.rotate(Eigen::AngleAxisd(t*2.*M_PI,Eigen::Vector3d(0,1,0)));
    return T;
  };

  Eigen::MatrixXd camera_v;
  Eigen::MatrixXi camera_f;
  Eigen::MatrixXd corner_normals;
  Eigen::MatrixXi fNormIndices;
  Eigen::MatrixXd UV_V;
  Eigen::MatrixXi UV_F;
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;

  // read obj file
  igl::readOBJ(std::string(TUTORIAL_SHARED_PATH) + "/sphere.obj",
          V, UV_V, corner_normals, F, UV_F, fNormIndices);

  // read fractal file
  string xyz_filepath = argv[1];
  cout << "Fractal xyz position filepath: " << xyz_filepath << endl;
  ifstream read_xyz("xyz_filepath");
  if (!read_xyz) {
    cout << "Reading file '" << xyz_filepath << "' failed." << endl;
    return 1;
  }
  bool ready = false;
  string s;
  int count = 0;
  double fractal[3000]; // TODO: Use dynamic allocation; arbitrary maximum set

  while (getline(xyz_filepath, s)) {
    cout << "reading xyz file..: " << s << endl;
    if (ready) {
      // X
      int ws1 = s.indexOf(' ');
      double d1 = s.substr(0, ws1);
      cout << "x: " << d1 << endl;
      // Y
      ws1 += 1;
      int ws2 = s.indexOf(' ', ws1);
      double d2 = s.substr(ws1, ws2 - ws1);
      cout << "y: " << d2 << endl;
      // Z
      ws2 += 1;
      double d3 = s.substr(ws2);
      cout << "z: " << d3 << endl;

      count++;
      fractal[3 * count] = d1;
      fractal[3 * count + 1] = d2;
      fractal[3 * count + 2] = d3;
    } else if (s == "X Y Z") {
      cout << "\nREADY!\n" << endl;
    }
  }

  // iterate through monomer locations
  int len = sizeof(fractal) / sizeof(double) - 3;
  for (int k=len; k>=0; k-=3) {
    // Create new data slot and set to selected
    /*if(!(viewer.data().F.rows() == 0  && viewer.data().V.rows() == 0)) {
      cout << "append mesh" << endl;
      viewer.append_mesh();
    }*/
    // viewer.data().clear();
    if (k == len) {
      viewer.data().set_mesh(V,F);
      viewer.data().set_uv(UV_V,UV_F);
      viewer.data().compute_normals();
    }
    viewer.data().uniform_colors(Eigen::Vector3d(30.0/255.0,30.0/255.0,30.0/255.0),
                                 Eigen::Vector3d(30.0/255.0,30.0/255.0,30.0/255.0),
                                 Eigen::Vector3d(100.0/255.0,100.0/255.0,100.0/255.0));
    viewer.append_mesh();

    // Translate
    Eigen::Affine3d T = Eigen::Affine3d::Identity();
    T.translate(Eigen::Vector3d(fractal[k] / 60, fractal[k+1] / 60, fractal[k+2] / 60));
  
    Eigen::MatrixXd VT = V * T.matrix().block(0,0,3,3).transpose();
    Eigen::RowVector3d trans = T.matrix().block(0,3,3,1).transpose();
    VT = (VT.rowwise() + trans).eval();
    viewer.data().set_mesh(VT, F);

    // Face-based
    viewer.data().set_face_based(true);
    
     // Wireframe
    viewer.data().show_lines = false;
    viewer.data().uniform_colors(Eigen::Vector3d(30.0/255.0,30.0/255.0,30.0/255.0),
                                 Eigen::Vector3d(30.0/255.0,30.0/255.0,30.0/255.0),
                                 Eigen::Vector3d(50.0/255.0,50.0/255.0,50.0/255.0));

    // viewer.data().clear();
    // viewer.data().set_vertices(VT);
    // viewer.data().compute_normals();
    // viewer.data_list.back().translate(Eigen::Vector3d(fractal[k] / 60, fractal[k+1] / 60, fractal[k+2] / 60));
  }
  
  // Set camera focus on center monomer
  viewer.core.camera_zoom = 0.1;

  //viewer.data().set_mesh(V, F);
  //viewer.data().set_face_based(true);
  viewer.core.is_animating = true;
  // viewer.data().compute_normals();
  
  const int grid_size = 50;
  const int time_steps = 200;
  const double isolevel = 0.1;

  viewer.callback_pre_draw = [&](igl::opengl::glfw::Viewer & viewer)->bool {
    for (auto& d : viewer.data_list) {
      if (viewer.core.is_animating) {
        Eigen::Affine3d T = transform(0.000000000001*igl::get_seconds());
        Eigen::MatrixXd VT = d.V*T.matrix().block(0,0,3,3).transpose();
        Eigen::RowVector3d trans = T.matrix().block(0,3,3,1).transpose();
        VT = ( VT.rowwise() + trans).eval();
        d.set_vertices(VT);
        d.compute_normals();
      }
      
      // Face-based
      d.set_face_based(viewer.data().face_based);

      // Show faces
      d.show_faces = viewer.data().show_faces;

      // Wireframe
      d.show_lines = viewer.data().show_lines;
      if (d.show_lines) {
        d.uniform_colors(Eigen::Vector3d(130.0/255.0,130.0/255.0,130.0/255.0),
                         Eigen::Vector3d( 90.0/255.0, 90.0/255.0, 90.0/255.0),
                         Eigen::Vector3d(200.0/255.0,200.0/255.0,200.0/255.0));
      } else {
        d.uniform_colors(Eigen::Vector3d( 30.0/255.0, 30.0/255.0, 30.0/255.0),
                         Eigen::Vector3d( 30.0/255.0, 30.0/255.0, 30.0/255.0),
                         Eigen::Vector3d( 50.0/255.0, 50.0/255.0, 50.0/255.0));
      }
    }
    return false;
  };

  // Launch GUI
  viewer.launch();
}
