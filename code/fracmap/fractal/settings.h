/* settings.h
 * Modified: Mark Garro and Shammah Chancellor (09/06/07)
 * Modified: Chris Hayes (07/03/18)
 */
#pragma once
#include <vector>
#include <cstdlib>
#include <math.h>
#include <ostream>
#include <time.h>

// #include "log2.hpp"

const std::string version = "07.12.18";

// Program Parameters (defaults)
const double fractal_dimension = 2.2;
const double prefactor = 1.13;
const double monomer_count = 10;
const double overlap = 1.0;
const double epsilon = 0.05;
const double mean_radius = 30.0;

const int max_random_monomers = 5000; // Exact iterations spent finding monomer with best
const int max_attach_vectors = 1000;  // Maximum number of random unit vectors to attempt to attach new monomer at.

const int precision = 40;
const int num_colors = 8;

const double pi = 3.14159;

//const int CORRELATION_PRECISION = 500;
const int parts_per_radius = 10;
//const double SHELL_RADIUS = 2.0 * MEAN_RADIUS / PARTS_PER_RADIUS;
//const double CORRELATION_INCR = MEAN_RADIUS / PARTS_PER_RADIUS / 2;
const double filter_depth = mean_radius;

// Output
const std::string run_output_filename = "run.txt";

// Logging
// static Log& _log = Log::get_instance();
