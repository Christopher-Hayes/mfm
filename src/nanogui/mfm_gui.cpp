#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/entypo.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/colorpicker.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <nanogui/nanogui.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

#if defined(_WIN32)
#  pragma warning(pop)
#endif
#if defined(_WIN32)
#  if defined(APIENTRY)
#    undef APIENTRY
#  endif
#  include <windows.h>
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::to_string;

using namespace nanogui;

static void fracmapCB();
static void modelCB();
static void addaCB();

// Fractal generation variables
double df = 2.6;	// Fractal dimension
double p = 1.13;	// Prefactor
int n = 50;		// Monomer count
double k = 0.7;		// Overlap factor
double r = 30.0;	// Radius (nm) of monomer
double e = 1.0;		// Epsilon (error)
std::string output_dir;	// Output directory for fractal data

// Light simulation variables
double lambda = 550;	// Wavelength of light (nm)
double dpl_size = 10;	// Dipole Size (nm)
double dpl = 55;	// Dipoles per lambda (dependent on dpl_size)
int grid = 3;		// Dipole grid size (dependent on dpl_size)
std::string adda_mode= "seq";	// ADDA mode (seq, mpi, ocl)

class MFMGUI : public nanogui::Screen {
  public:
      MFMGUI() : nanogui::Screen(Eigen::Vector2i(1024, 768), "MFM") {
        // WINDOW: FracMAP -------------------------------------------
        Window *windowFracMAP = new Window(this, "FracMAP Options");
        windowFracMAP->setPosition(Vector2i(15, 15));
        windowFracMAP->setLayout(new GroupLayout());

        // WINDOW: Light Simulation ----------------------------------
        /*Window *windowLight = new Window(this, "Light Simulation Options");
        windowLight->setPosition(Vector2i(85, 15));
        windowLight->setLayout(new GroupLayout());*/

        // WINDOW: Action Buttons -------------------------------------
        Window *windowAction = new Window(this, "Run Action");
        windowAction->setPosition(Vector2i(15, 55));
        windowAction->setLayout(new GroupLayout());

        runFracMAP = new Button(windowAction, "Run FracMAP");
        runFracMAP->setCallback(&fracmapCB);
        runModel = new Button(windowAction, "Run FracMAP + Show 3D Model");
        runADDA = new Button(windowAction, "Run FracMAP + Run ADDA");


        performLayout();
      }

    virtual void draw(NVGcontext *ctx) {
        /* Animate the scrollbar */
        //mProgress->setValue(std::fmod((float) glfwGetTime() / 10, 1.0f));

        /* Draw the user interface */
        Screen::draw(ctx);
    }

  private:
      // --- Members ----------------------------------------------------------
      // FracMAP options
      // Light Simulation options
      // Action Buttons
      Button *runFracMAP, *runModel, *runADDA;
      // --- Functions --------------------------------------------------------
};

// --- FracMAP ----------------------------------------------------------------
static void fracmapCB() {
  // Build fractal aggregate with MFM
  string command = "code/fracmap/fractal/fracmap ";
  command += " -d " + to_string(df);
  command += " -p " + to_string(p);
  command += " -n " + to_string(n);
  command += " -k " + to_string(k);
  command += " -r " + to_string(1);
  command += " -e " + to_string(e);
  if (output_dir != "") {
    command += " -t " + output_dir;
  }
  system(command.c_str());

  // Show 3D model
  int status = system(command.c_str());

  // Get return value (output folder) and send output to ADDA
  if (status < 0) {
    std::cout << "Error: " << strerror(errno) << '\n';
  } else {
    string filename;
    std::ifstream read_output_filename("logs/most_recent_output_filename.txt");
    read_output_filename >> filename;
    read_output_filename.close();

    std::cout << "MOST RECENT OUTPUT FILENAME = " << filename << std::endl;

    string command = "src/libigl/main " + filename;
    cout << "COMMAND PATH: " << command << endl;
    system(command.c_str());
  }
}
// --- Model ------------------------------------------------------------------
static void modelCB() {
}
// --- ADDA -------------------------------------------------------------------
static void addaCB() {
}

// --- main -------------------------------------------------------------------
int main(int /* argc */, char ** /* argv */) {
    try {
        nanogui::init();

        {
            nanogui::ref<MFMGUI> app = new MFMGUI();
            app->drawAll();
            app->setVisible(true);
            nanogui::mainloop();
        }

        nanogui::shutdown();
    } catch (const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        #if defined(_WIN32)
            MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
        #else
            std::cerr << error_msg << endl;
        #endif
        return -1;
    }

    return 0;
}

int main_old(int /* argc */, char ** /* argv */) {
  try {
    nanogui::init();

    {
      // OpenGL 4.1 context
        bool use_gl_4_1 = false;
        Screen *screen = nullptr;

        if (false && use_gl_4_1) {
          /*
          screen = new Screen(Vector2i(500, 700), "NanoGUI test [GL 4.1]",
                              /resizable/true, /fullscreen/false, /colorBits/8,
                              /alphaBits/8, /depthBits/24, /stencilBits/8,
                              /nSamples/0, /glMajor/4, /glMinor/1);
                              */
        } else {
          screen = new Screen(Vector2i(500, 700), "Fractal Parameters");
        }

        bool enabled = true;
        FormHelper *gui = new FormHelper(screen);
        ref<Window> window = gui->addWindow(Eigen::Vector2i(10, 10), "Parameters");
        gui->addGroup("Fractal Generation");
        gui->addVariable("Fractal Dimension", df)->setSpinnable(true);
        gui->addVariable("Prefactor", p)->setSpinnable(true);
        gui->addVariable("Monomer Count", n)->setSpinnable(true);
        gui->addVariable("Overlap Factor", k)->setSpinnable(true);
        gui->addVariable("Monomer Diameter", r)->setSpinnable(true);
        gui->addVariable("epsilon", e)->setSpinnable(true);
        gui->addVariable("Output Folder", output_dir);

        // Show Model
        gui->addButton("Show Model", []() {
            // Build fractal aggregate with MFM
            string command = "code/fracmap/fractal/fracmap ";
            command += " -d " + to_string(df);
            command += " -p " + to_string(p);
            command += " -n " + to_string(n);
            command += " -k " + to_string(k);
            command += " -r " + to_string(1);
            command += " -e " + to_string(e);
            if (output_dir != "") {
              command += " -t " + output_dir;
            }
            system(command.c_str());

            // Show 3D model
            int status = system(command.c_str());

            // Get return value (output folder) and send output to ADDA
            if (status < 0) {
              std::cout << "Error: " << strerror(errno) << '\n';
            } else {
              string filename;
              std::ifstream read_output_filename("logs/most_recent_output_filename.txt");
              read_output_filename >> filename;
              read_output_filename.close();

              std::cout << "MOST RECENT OUTPUT FILENAME = " << filename << std::endl;

              string command = "src/libigl/main " + filename;
              cout << "COMMAND PATH: " << command << endl;
              system(command.c_str());
            }
        });

        gui->addGroup("Light Analysis");

        gui->addVariable("Lambda (nm)", lambda)->setSpinnable(true);
        gui->addVariable("Dipole Size (nm)", dpl_size)->setSpinnable(true);
        gui->addVariable("ADDA Mode (seq, mpi)", adda_mode)->setEditable(false);

        // Send Model Data to ADDA
        gui->addButton("Send to ADDA", []() {
            // Run fracmap command
            string command = "code/fracmap/fractal/fracmap ";
            command += " -d " + to_string(df);
            command += " -p " + to_string(p);
            command += " -n " + to_string(n);
            command += " -k " + to_string(k);
            command += " -r " + to_string(1);
            command += " -e " + to_string(e);
            if (output_dir != "") {
              command += " -t " + output_dir;
            }
            int status = system(command.c_str());

            // Get return value (output folder) and send output to ADDA
            if (status < 0) {
              std::cout << "Error: " << strerror(errno) << '\n';
            } else {

              string filename;
              std::ifstream read_output_filename("logs/most_recent_output_filename.txt");
              read_output_filename >> filename;
              read_output_filename.close();

              std::cout << "MOST RECENT OUTPUT FILENAME = " << filename << std::endl;

              // Run FracMAP -> ADDA command
	      grid = round(r / dpl_size);
              string command = "python3 code/mfm_2_adda/batchGenDipoles.py "
		      		+ filename
				+ " " + to_string(dpl_size)
				+ " " + to_string(grid)
				+ " " + to_string(lambda / 1000)
				+ " " + adda_mode;
              cout << "COMMAND PATH: " << command << endl;
              system(command.c_str());
            }

        });


        screen->setVisible(true);
        screen->performLayout();
        window->center();

        nanogui::mainloop();
    }

    nanogui::shutdown();
  } catch (const std::runtime_error &e) {
    std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
    #if defined(_WIN32)
        MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
    #else
        std::cerr << error_msg << endl;
    #endif
    return -1;
  }

  return 0;
}
