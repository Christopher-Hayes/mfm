/* main.cpp - Program entry point.
 * Created:  Mark Garro  (09/06/07)
 * Modified: Chris Hayes (06/26/18)
 */
#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <cstring>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "fractal.h"
#include "log2.hpp"
#include "settings.h"
#include "micrograph.h"
#include "params.h"

#include <boost/filesystem.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

using namespace std;

// Print function menu ---------------------------------------------------------
void 
print_menu() {
  LOGGABLE;
	cout << "\n  + - - - - - - - FracMAP  menu - - - - - - - +"
       << "\n  |     " << left << setw(38) << "" << "|"
       << "\n  | 0 | " << setw(38) << "Create new fractal (Clear prexisting)" << "|"
       << "\n  |     " << setw(38) << "" << "|"
       << "\n  | 1 | " << setw(38) << "Calculate structure factor of fractal" << "|"
       << "\n  |     " << setw(38) << "" << "|"
       << "\n  | 2 | " << setw(38) << "Run 2D Micrograph analysis on fractal" << "|"
       << "\n  |     " << setw(38) << "" << "|"
       << "\n  | 3 | " << setw(38) << "Quit" << "|"
	     << "\n  + - - - - - - - - - - - - - - - - - - - - - +"
       << "\n\n  Choose one.\n  Input the integer and press enter: ";
}
// Open output file ------------------------------------------------------------
void open_output(ofstream& output) {
  string filename = "";

  // Open ofstream
  for (;;) {
    cout << "Output File Name? (e.g. output.txt)" << endl;
    cin >> filename;
    // Check that file doesn't already exists. access() is a sys/stat.h function
    if (access(filename.c_str(), F_OK) == -1)
      break;
    WARN << "file already exists.";
  }

  output.open(filename.c_str());
}
// Compute structure factor ----------------------------------------------------
void
struct_factor(Fractal& base) {
  // Get output file
  ofstream output;
  open_output(output);

  // Compute structure factor
  base.structurec(output);

  output.close();
}
// Compute 2D micrograph analysis ----------------------------------------------
void
micro_analysis(Fractal& base) {
  // Get output file
  ofstream output;
  open_output(output);

  // Compute 2D micrograph analysis
  base.orient_random(output, 25, true);

  output.close();
}
// Input Validation ============================================================
// Extract double input
double
grab_double(const double& def, string param, string range) {
  string input = "";
  try {
    cout << endl << param << " Range=" << range << " Default=" << def << "\n  : ";
    getline(cin, input);
    if (input == "")
      return def;
    else
      return stod(input);
  }catch (const exception& e) {
    FATAL << "Bad double input."; // improve...
    return -1.0;
  }
}
// Extract integer input
int
grab_int(const int& def, string param, string range) {
  string input = "";
  try {
    cout << endl << param << " Range=" << range << " Default=" << def << "\n  : ";
    getline(cin, input);
    if (input == "")
      return def;
    else
      return stoi(input);
  }catch (const exception& e) {
    FATAL << "Bad int input."; // improve..
    return -1;
  }
}
// Validate Fractal Dimension --------------------------------------------------
void
validate_df(double& df, bool cli=false) {
  // Prompt for value if not passed in CLI
  if (!cli)
    df = grab_double(fractal_dimension, "Fractal Dimension", "[1.0, 3.0]");
  for (;;) {
    // Check value is within bounds
    if (df >= 1.0 && df <= 3.0)
      break;
    WARN << "Fractal Dimension is invalid. It must be a double within the bounds [1.0, 3.0]\n";
    cout << "New Fractal Dimension: ";
    df = grab_double(fractal_dimension, "Fractal Dimension", "[1.0, 3.0]");
  }
  INFO << "Good input: fractal dimension";
}
// Validate Prefactor ----------------------------------------------------------
void
validate_kf(double& kf, bool cli=false) {
  // Prompt for value if not passed in CLI
  if (!cli)
    kf = grab_double(prefactor, "Prefactor", "[1.0, inf)");
  for (;;) {
    // Check value is within bounds
    if (kf >= 1.0)
      break;
    WARN << "Prefactor is invalid. It must be a double within the bounds [1.0, inf)\n";
    cout << "New Prefactor: ";
    kf = grab_double(prefactor, "Prefactor", "[1.0, inf)");
  }
  INFO << "Good input: prefactor";
}
// Validate Monomer Count ------------------------------------------------------
void
validate_n(int& n, bool cli=false) {
  // Prompt for value if not passed in CLI
  if (!cli)
    n = grab_int(monomer_count, "Monomer Count", "[1, inf)");
  for (;;) {
    // Check value is within bounds
    if (n > 0)
      break;
    WARN << "Monomer count is invalid. It must be an integer with the bounds [1, inf)\n";
    cout << "New Monomer Count: ";
    n = grab_int(monomer_count, "Monomer Count", "[1, inf)");
  }
  INFO << "Good input: monomer count";
}
// Validate Overlap Factor -----------------------------------------------------
void
validate_k(double& k, bool cli=false) {
  // Prompt for value if not passed in CLI
  if (!cli)
    k = grab_double(overlap, "Overlap Factor", "[0.5, 1.0]");
  for (;;) {
    // Check value is within bounds
    if (k >= 0.5 && k <= 1.0)
      break;
    WARN << "Overlap Factor is invalid. It must be a decimal with the bounds [0.5, 1.0]\n";
    cout << "New Overlap Factor: ";
    k = grab_double(overlap, "Overlap Factor", "[0.5, 1.0]");
  }
  INFO << "Good input: overlap factor";
}
// Validate Epsilon ------------------------------------------------------------
void
validate_e(double& e, bool cli=false) {
  string input;
  // Prompt for value if not passed in CLI
  if (!cli)
    e = grab_double(epsilon, "Epsilon", "(0.0, inf)");
  for (;;) {
    // Check value is within bounds
    if (e > 0.0)
      break;
    WARN << "Epsilon is invalid. It must be a decimal greater than 0.0\n";
    cout << "New Epsilon (press enter to use default): ";
    e = grab_double(epsilon, "Epsilon", "(0.0, inf)");
  }
  INFO << "Good input: overlap factor";
}
// Program entry point ---------------------------------------------------------
int
main(int argc, char **argv) {
  Log log;
  srand((unsigned int)time(NULL));

  // ofstream out("out.txt");

	Fractal* base = nullptr;
  double df = -1; // Fractal Dimension
  double kf = -1; // Prefactor
  int n = -1; // Monomer Count
  double k = -1; // Overlap Factor
  double e = -1; // Epsilon
  string s; // for input
  Params p = Params(argc, argv);
  // Quit out if usage/help parameter is active.
  if (p.check_usage())
    return 0;
  // Verbose
  if (p.check_verbose()) {
    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );
  }

  // Fractal Dimension
  if (p.check_df()) {
    df = p.get_df();
    validate_df(df, true);
    INFO << "CLI argument df=" + to_string(df);
  }else if (p.check_def()) {
    df = fractal_dimension;
    INFO << "Using default value for fractal dimension. df=" + to_string(df);
  }
  // Prefactor
  if (p.check_kf()) {
    kf = p.get_kf();
    validate_kf(kf, true);
    INFO << "CLI argument kf=" + to_string(kf);
  }else if (p.check_def()) {
    kf = prefactor;
    INFO << "Using default value for prefactor. kf=" + to_string(kf);
  }
  // Monomer Count
  if (p.check_n()) {
    n = p.get_n();
    validate_n(n, true);
    INFO << "CLI argument n=" + to_string(n);
  }else if (p.check_def()) {
    n = monomer_count;
    INFO << "Using default value for monomer count. n=" + to_string(n);
  }
  // Overlap Factor
  if (p.check_k()) {
    k = p.get_k();
    validate_k(k, true);
    INFO << "CLI argument k=" + to_string(k);
  }else if (p.check_def()) {
    k = overlap;
    INFO << "Using default value for overlap. k=" + to_string(k);
  }
  // Epsilon
  if (p.check_e()) {
    e = p.get_e();
    validate_e(e, true);
    INFO << "CLI argument e=" + to_string(e);
  }else if (p.check_def()) {
    e = epsilon;
    INFO << "Using default value for epsilon. e=" + to_string(e);
  }

  // Runs
  if (p.check_r()) {
    INFO << "Running batch..";
    // Validation
    if (!p.check_df() && !p.check_def())
      validate_df(df);
    if (!p.check_kf() && !p.check_def())
      validate_kf(kf);
    if (!p.check_n() && !p.check_def())
      validate_n(n);
    if (!p.check_k() && !p.check_def())
      validate_k(k);
    if (!p.check_e() && !p.check_def())
      validate_e(e);
    // # runs
    int total_runs = p.get_r();
    if (total_runs < 1)
      FATAL << "Invalid number of runs. Must be greater than 0.";
    // TODO: ofstream safety
    // Create run_output directory (if applicable)
    if (!boost::filesystem::exists("./run_output"))
        boost::filesystem::create_directory("./run_output");
    // Create batch directory; TODO: naming conflicts
    // TODO: Figure out a way to move logging into this directory (would be logical)
    string batch_dir = "";
    if (p.check_run_output()) {
      // TODO: interface path operations through boost filestream library
      batch_dir = string("./run_output/run_") + p.get_run_output();
      boost::filesystem::create_directory(batch_dir);
    }else {
      // Current date-time will be directory name
      boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%Y%m%d_%H:%M:%S.%f");
      stringstream date_stream;
      date_stream.imbue(std::locale(date_stream.getloc(), facet));
      date_stream << boost::posix_time::microsec_clock::universal_time();
      batch_dir = string("./run_output/run_") + date_stream.str();
      // Create directory
      boost::filesystem::create_directory(batch_dir);
    }
    // Output file
    ofstream run_output(batch_dir + "/run.txt");
    run_output << "Df\tkf\tn\tk\te\n"
               << df << "\t" << kf << "\t" << n << "\t"
               << k << "\t" << e << "\n\n"
               << "Run\tRg\tActual_Epsilon" << endl;
    // Runs
    double sum_rg = 0;
    double sum_e = 0;
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    string xyz_filepath = "";
    for (int run=1; run<total_runs+1; run++) {
      // Progress bar
      if (run % 10 == 0 || run == total_runs) {
        int prog = floor(double(run) / (double)total_runs * 49);
        cout << log_green << "Run " << run << "\t<"
             << setfill('#') << setw(prog) << ""
             << log_red << setfill('_') << setw(49 - prog) << ""
             << log_green << ">  " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count()) << " ms" << endl;
      }
      // Build fractal
      base = new Fractal(df, kf, k, e);
      base->generate_fractal(n, false);
      // Print fractal output
      run_output << run << "\t" << base->rg() << "\t" << base->actual_e() << endl;
      sum_rg += base->rg();
      sum_e += base->actual_e();
      // Output XYZ
      xyz_filepath = batch_dir + "/xyz_run_" + to_string(run) + ".txt";
      INFO << "Location xyz_filepath: " << xyz_filepath << endl;
      ofstream xyz_output(xyz_filepath);
      base->print_monomers(xyz_output);
      xyz_output.close();
    }
    // Averages
    run_output << "-----------------\nAverage Rg: " << (sum_rg / total_runs)
               << "\nAverage Actual Epsilon: " << (sum_e / total_runs) << endl;
    run_output.close();
    cout << log_reset << "Batch run completed successfully in "
         << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count()) << " ms" << endl;

    ofstream output_filename("most_recent_output_filename.txt");
    output_filename << xyz_filepath.substr(1);
    output_filename.close();

    // Visuals TODO: remove this terrible cross-application spaghetti mess
    //string command = "code/libigl/build/tutorial/102_DrawMesh_bin ." + xyz_filepath.substr(1);
    //cout << "COMMAND PATH: " << command << endl;
    //system(command.c_str());
  }else {
    // Program loop; on first run automatically run new fractal
    for (char key = '0';;) {
      switch (key)
      {
        case '0': // Validate new params
          // Validate / get params
          validate_df(df);
          validate_kf(kf);
          validate_n(n);
          validate_k(k);
          validate_e(e);
          INFO << "Creating new fractal..";
          // Generate fractal
          base = new Fractal(df, kf, k, e);
          base->generate_fractal(n);
          // base->print_monomers(out);
          // out.close();
          // Reset parameters for next iteration
          df = kf = k = e = -1.0;
          n = -1;
          break;
        case '1': // Structure Factor
          INFO << "Computing the fractal's stucture factor..";
          struct_factor(*base);
          break;
        case '2': // Micrograph analysis
          INFO << "Computing a 2D micrograph analysis on the fractal..";
          micro_analysis(*base);
          break;
        case '3': // Quit
          if (base != nullptr)
            delete base;
          cout << "Program terminated successfully.\n" << endl;
          return 0;
        default: // Catch all invalid input
          WARN << "Invalid menu option.\n";
          break;					
      }
      // Print main menu; get user input
      print_menu();
      cin >> key;
      // Ignore extraneous output
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
  }
	return 0;
}
