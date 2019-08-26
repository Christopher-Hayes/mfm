/* params.cpp
 * Modified: Chris Hayes (07/04/18)
 */
#include <cstdlib>
#include <string>

#include "params.h"
using namespace std;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// constructor
Params::Params(int argc, char* argv[])
{
  // init
  _df_v = _kf_v = _k_v = _e_v = 0.0;
  _n_v = 0;
  _verbose = _vec_output = _run_output = _usage = _def = _df = _kf = _n = _k = _r = _e = false;
  // process switches
  int code, opt;
  while ((opt = getopt_long(argc, argv, "vbhfd:p:n:k:e:r:o:t:", LONG_OPTS, &code)) != -1){
    switch (opt) {
      case 'd': // Fractal Dimension
        _df = true;
        _df_v = extract_double("Fractal Dimension");
        break;
      case 'p': // Prefactor
        _kf = true;
        _kf_v = extract_double("Prefactor");
        break;
      case 'n': // Monomer Count
        _n = true;
        _n_v = extract_int("Monomer Count");
        break;
      case 'k': // Overlap factor
        _k = true;
        _k_v = extract_double("Overlap Factor");
        break;
      case 'e': // Epsilon
        _e = true;
        _e_v = extract_double("Epsilon");
        break;
      case 'f': // Defaults
        _def = true;
        break;
      case 'r': // Runs
        _r = true;
        _r_v = extract_int("Runs");
        break;
      case 'b': // Verbose
        _verbose = true;
        break;
      case 'o': // Monomer XYZ output
        _vec_output = true;
        _vec_output_file = optarg;
        break;
      case 't': // Run result output
        _run_output = true;
        _run_output_file = optarg;
        break;
      case 'v': // Version
        _usage = true;
        cout << "fracMAP v" << log_green << version << log_reset << endl;
        break;
      case 'h': // Help
        _usage = true;
        print_usage();
        break;
      case '?':
      default:
        print_usage(1);
    }
  }
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Extract float from optarg
double Params::
extract_double(string parameter) {
  try {
    return stod(string(optarg)); // stod() used instead of atof() for exceptions
  }catch (const exception& e) {
    print_usage();
    FATAL << "Invalid argument for " + parameter + ": " + e.what();
    return -1.0; // remove no return warnings
  }
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Extract integer from optarg
int Params::
extract_int(string parameter) {
  try {
    string s = string(optarg);
    if ((double)stoi(s) != stod(s))
      FATAL << parameter + " cannot be a decimal. Must be integer.";
    return stoi(s);
  }catch (const exception& e) {
    print_usage();
    FATAL << "Invalid argument for " + parameter + ". Must be integer.";
    return -1; // remove no return warnings
  }
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// print usage; err defaults to -1 (no fatal error)
void
Params::print_usage(int err)
{
  // print usage; modeled from nmap's usage comment
  cout << "\n\tFracMAP v" << log_green << version << log_reset
       << "\n\nUsage:  ./fracmap [Options]"
       << "\n\nInline Command Arguments (program will prompt in absence)"
       // Fractal Dimension
       << "\n\n" << " Fractal Dimension"
       << "\n  " << log_cyan << "-d <fractal dimension>"
       << log_magenta << " --fractal_dimension <fractal dimension>" << log_reset
       << "\n  Expects " << log_blue << "decimal value " << log_reset << "with the range of [1.0, 3.0]"
       << log_cyan << "\n    Default=" << fractal_dimension << log_reset
       // Prefactor
       << "\n\n Prefactor"
       << "\n  " << log_cyan << "-p <prefactor>"
       << log_magenta << " --prefactor <prefactor>" << log_reset
       << "\n  Expects " << log_blue << "decimal value " << log_reset << "with the range of [1.0, inf)"
       << log_cyan << "\n    Default=" << prefactor << log_reset
       // Monomer Count
       << "\n\n Monomer Count"
       << "\n  " << log_cyan << "-n <monomer count>"
       << log_magenta <<" --monomers <monomer count>" << log_reset
       << "\n  Expects " << log_green << "integer value " << log_reset << "greater than 0"
       << log_cyan << "\n    Default=" << monomer_count << log_reset
       // Overlap Factor
       << "\n\n Overlap Factor"
       << "\n  " << log_cyan << "-k <overlap factor>"
       << log_magenta <<  " --overlap <overlap factor>" << log_reset
       << "\n  Expects " << log_blue << "decimal value " << log_reset << "with the range of [0.5, 1.0]"
       << log_cyan << "\n    Default=" << overlap << log_reset
       // Epsilon
       << "\n\n Epsilon"
       << "\n  " << log_cyan << "-e <epsilon>"
       << log_magenta <<  " --epsilon <epsilon>" << log_reset
       << "\n  Expects " << log_blue << "decimal value " << log_reset << "with the range of (0.0, inf)"
       << log_cyan << "\n    Default=" << epsilon << log_reset
       // Defaults
       << "\n\n Defaults"
       << "\n  " << log_cyan << "-f"
       << log_magenta <<  " --defaults" << log_reset
       << "\n  Automatically use parameter defaults if not explicitly set in CLI argument."
       << "\n\n\nBATCH: "
       // Number of runs
       << "\n\n Runs"
       << "\n  " << log_cyan << "-r <number runs>"
       << log_magenta << " --runs <number runs>" << log_reset
       << "\n Run FracMAP a number of times with the same parameters. Expects integer greater than 0."
       << "\n\n\nOUTPUT:"
       // Verbose
       << "\n\n Verbose"
       << "\n  " << log_cyan << "-b"
       << log_magenta << " --verbose" << log_reset
       << "\n  Verbose output"
       // Monomer XYZ output
       << "\n\n Monomer XYZ Output"
       << "\n  " << log_cyan << "-o <destination>"
       << log_magenta << " --vec_output <destination>" << log_reset
       << "\n  Log monomer XYZ centers to file."
       // Run output
       << "\n\n Multiple Run File Output"
       << "\n  " << log_cyan << "-t <destination>"
       << log_magenta << " --run_output <destination>" << log_reset
       << "\n  Choose filename to output run result. Program puts file inside the ./run_output directory"
       << "\n\n\nMISC:"
       // Help / Usage
       << "\n\n Help"
       << "\n  " << log_cyan << "-h"
       << log_magenta << " --help --usage" << log_reset
       << "\n  Print this help summary page"
       // Version
       << "\n\n Version"
       << "\n  " << log_cyan << "-v"
       << log_magenta << " --version" << log_reset
       << "\n  Get program version"
       << "\n\n\nEXAMPLES:"
       << "\n\n  ./fracmap -d 1.8 -p 3.4 -n 13 -k 0.8 -o out.txt"
       << "\n\n  ./fracmap --fractal_dimension 2.4 --output output.txt -b\n\n" <<endl;
  // specific fatal error
  switch (err)
  {
    case 0:
      FATAL << "ERROR: Missing required argument.\n";
    case 1:
      FATAL << "ERROR: Invalid switch.\n";
    case 2:
      FATAL << "ERROR: Invalid required argument.\n";
  }
}
