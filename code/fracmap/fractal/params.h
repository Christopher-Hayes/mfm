/* params.h
 * Modified: (07/04/18) Chris Hayes
 */
#pragma once
#include <iostream>
#include <getopt.h>

#include "settings.h"
#include "log2.hpp"

class Params
{
public:
  // Constructors, Deconstructors
  Params() {}
  Params(int, char**);
  // Print usage comment
  static void print_usage(int err = -1);
  // Extract optarg safely
  double extract_double(std::string parameter);
  int extract_int(std::string parameter);
  // Check if param active
  inline bool check_df() const { return _df; }
  inline bool check_kf() const { return _kf; }
  inline bool check_n() const { return _n; }
  inline bool check_k() const { return _k; }
  inline bool check_e() const { return _e; }
  inline bool check_r() const { return _r; }
  inline bool check_vec_output() const { return _vec_output; }
  inline bool check_run_output() const { return _run_output; }
  inline bool check_verbose() const { return _verbose; }
  inline bool check_def() const { return _r; }
  inline bool check_usage() const { return _usage; }
  // Get param argument
  inline double get_df() const { return _df_v; }
  inline double get_kf() const { return _kf_v; }
  inline int get_n() const { return _n_v; }
  inline double get_k() const { return _k_v; }
  inline double get_e() const { return _e_v; }
  inline int get_r() const { return _r_v; }
  inline const char* get_vec_output() const { return _vec_output_file; }
  inline const char* get_run_output() const { return _run_output_file; }

private:
  // Option values
  const char* _vec_output_file = ""; // const for c++11+ compat.
  const char* _run_output_file = ""; // const for c++11+ compat.
  double _df_v, _kf_v, _k_v, _e_v;
  int _n_v, _r_v;
  bool _verbose, _vec_output, _run_output, _df, _kf, _n, _k, _e, _r, _def, _usage;

  // Long CLI options
  struct option LONG_OPTS[14] = {
    { "verbose",            no_argument,        NULL,  'b' },
    { "fractal_dimension",  required_argument,  NULL,  'd' },
    { "prefactor",          required_argument,  NULL,  'p' },
    { "monomers",           required_argument,  NULL,  'n' },
    { "overlap",            required_argument,  NULL,  'k' },
    { "epsilon",            required_argument,  NULL,  'e' },
    { "defaults",           required_argument,  NULL,  'f' },
    { "vec_output",         required_argument,  NULL,  'o' },
    { "runs",               required_argument,  NULL,  'r' },
    { "run_output",         required_argument,  NULL,  't' },
    { "help",               no_argument,        NULL,  'h' },
    { "usage",              no_argument,        NULL,  'h' },
    { "version",            no_argument,        NULL,  'v' },
    { NULL,                 0,                  NULL,   0  }
  };

};
