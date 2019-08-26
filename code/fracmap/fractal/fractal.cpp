/* fractal.cpp
 * Created:  Mark Garro  (09/06/07)
 * Modified: Chris Hayes (06/26/18)
 */
#include "fractal.h"
using namespace std;

// Constructors / Deconstructors ===============================================
// Fractal constructor with Fractal Dimension, PreFactor, Overlap passed -------
Fractal::
Fractal(double df, double kf, double k, double e)
: _df(df),
  _kf(kf),
  _a(mean_radius),
  _k(k),
  _e(e),
  _r_sum(Vector_3(0, 0, 0)),
  _r_squared(0),
  _r_mean(Vector_3(0, 0, 0)),
  _rg(0),
  _n(0),
  _sum_epsilon(0)
{
  // Validate parameters
  if (df < 1.0 || df > 3.0)
    FATAL << "Invalid fractal dimension. Range: [1.0, 3.0]";
  if (kf < 1.0)
    FATAL << "Invalid prefactor. Range: [1.0, inf)";
  if (k < 0.5 || k > 1.0)
    FATAL << "Invalid overlap. Range: [0.5, 1.0]";
  INFO << "Fractal initialized successfully. df, kf, k are good.";
}

// Fractal Generation ==========================================================
// Generate fractal ------------------------------------------------------------
void Fractal::
generate_fractal(int target_size, bool output) {
  if (target_size < 1)
    FATAL << "Cannot generate fractal with a size less than 1.";
  INFO << "Generating fractal..";
  std::chrono::steady_clock::time_point start = chrono::steady_clock::now();
  while ((int)size() < target_size) {
    create_monomer();
    if (output)
      cout << "Fractal size: " << (int)size() << " of " << target_size << endl;
  }
  if (output) {
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << log_magenta << "\nFractal successfully generated." << endl;
    double ms = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
    // Print info
    cout << "\n" << setw(22) << right << "Time (ms): " << fixed << setprecision(8) << ms
         << "\n" << setw(22) << "Df: " << _df
         << "\n" << setw(22) << "kf: " << _kf
         << "\n" << setw(22) << "k: " << _k
         << "\n" << setw(22) << "n: "<< _n
         << "\n" << setw(22) << "Rg: " << _rg
         << "\n" << setw(22) << "Epsilon: " << epsilon
         << "\n" << setw(22) << "Avg Actual Epsilon: "
         << (_sum_epsilon / _n) << log_reset << endl;
  }
}
// Add new monomer -------------------------------------------------------------
void Fractal::
create_monomer() {
	if (_fractal.empty()) {
    INFO << "Adding fractal monomer center at (0.0, 0.0, 0.0)";
		add_monomer(0.0f, 0.0f, 0.0f);
	}else if (_fractal.size() == 1) {
    INFO << "Attaching random monomer to center monomer.";
	  add_monomer(find_attach_monomer(_fractal.front()));
	}else {
    INFO << "Adding new monomer to fractal using Monte Carlo algorithm.";
		monte_carlo();
	}
}
// Add monomer using x,y,z coordinates -----------------------------------------
void Fractal::
add_monomer(double x, double y, double z) {
	add_monomer(Vector_3(x, y, z));
}
// Append monomer; update r and r^2 sums ---------------------------------------
void Fractal::
add_monomer(const Vector_3& new_monomer) {
  _fractal.push_back(new_monomer);
  INFO << "Adding monomer at point " + new_monomer.print();
  
  _r_sum += new_monomer;
  _r_squared += new_monomer * new_monomer;

  parameter_update();
}
// Create monomer, randomly attach to factal such that it does not overlap
// beyond the accepted parameters ----------------------------------------------
Vector_3 Fractal::
find_attach_monomer(const Vector_3& parent_monomer) {
  Vector_3 new_monomer;
  Vector_3 rand_vector;
  double threshold = 2 * _a * _k;

    // Scale random unit vector to 2x radius, translate to random monomer
    // TODO: should get_random_monomer() be run every single time?

  // Repeat until monomer is above threshold distance from nearest monomer
  for (int attempt=0; attempt<max_attach_vectors; attempt++) {
    rand_vector = Vector_3::random_vec();
    new_monomer = rand_vector * threshold + parent_monomer;
    if (!monomer_proximity(new_monomer, threshold))
      return new_monomer;
  }
  // Failed -> return out of range value
  INFO << "Failed to attach to " + parent_monomer.print() + ". Trying new monomer.";
  double out_of_range = _n * _a;
  return Vector_3(out_of_range, out_of_range, out_of_range);
}
// MonteCarlo add monomer ------------------------------------------------------
void Fractal::
monte_carlo() {
	Vector_3 temp_monomer, parent_monomer;
  double out_of_range = _n * _a;
  INFO << "Out of range best_monomer = " + to_string(out_of_range);
  Vector_3 best_monomer(out_of_range, out_of_range, out_of_range);

	double temp_rg;
	double expected_rg = pow(((_n + 1) / _kf), 1 / _df) * _a;
  double best_rg = expected_rg * 1000; // Initialized out of range	
  double best_diff = abs(best_rg - expected_rg);
  bool under_epsilon = false;

  // Random monomer to attach to.
	for (int attempt = 0; attempt < max_random_monomers; attempt++) {
    parent_monomer = get_random_monomer();
	  temp_monomer = find_attach_monomer(parent_monomer);
	  temp_rg = test_rg(temp_monomer);
    // Check if monomer is within expected parameters
		if (abs(temp_rg - expected_rg) < best_diff) {
		  best_rg = temp_rg;
		  best_monomer = temp_monomer;
      best_diff = abs(best_rg - expected_rg);
		}
    // TEST: See how many iterations it takes to reach value under Epsilon
    if (!under_epsilon && (best_diff / _a) < _e) {
      INFO << "Under epsilon in " + to_string(attempt+1) + " attempts.";
      under_epsilon = true;
    }
	}

  // Epsilon
  double actual_epsilon = best_diff / _a;
  _sum_epsilon += actual_epsilon;
  if (actual_epsilon < _e) {
    INFO << string("Actual epsilon = ") + to_string(actual_epsilon);
    add_monomer(best_monomer);
  }else {
    FATAL << string("Best monomer did not satisfy epsilon.") +
               "\n  best_diff / _a = " + to_string(best_diff / _a) +
               "\n  epsilon = " + to_string(_e);
  }
}
// Find how a monomer would affect radius of gyration --------------------------
double Fractal::
test_rg(const Vector_3& test_monomer) {
  Vector_3 temp_r_mean = (_r_sum + test_monomer) / (_n + 1);
  double temp_r_squared = _r_squared + test_monomer * test_monomer;
  
  return sqrt(temp_r_squared / (_n + 1) - (temp_r_mean * temp_r_mean));
}
// Update _rg from sum of radii and r_mean -------------------------------------
// TBD: keep this? - increased efficiency vs opaque parameters + prone to bugs
void Fractal::
parameter_update() {
  _n = (int)_fractal.size();
  _r_mean = _r_sum / _n;
  _rg = sqrt(_r_squared / _n - (_r_mean * _r_mean)); // Standard deviation
  INFO << "Fractal parameters updated."; //May remove depending on speed cost
}
// Obtain a random monomer from the agglomerate --------------------------------
Vector_3 Fractal::
get_random_monomer() {
	return _fractal[rand() % _fractal.size()];
}
// Returns true if new monomer within threshold distance of another ------------
bool Fractal::
monomer_proximity(const Vector_3& new_monomer, double threshold) {
  for (const auto& monomer : _fractal) {
		if (new_monomer.distance(monomer) < threshold)
			return true; // Overlap found
	}
	return false;
}
// Clear fractal ---------------------------------------------------------------
void Fractal::clear() {
	_fractal.clear();
	parameter_update();
  INFO << "Fractal cleared.";
}

// Print functions =============================================================
// Print monomer XYZ centers ---------------------------------------------------
void Fractal::
print_monomers(ostream& out) const {
  out << "Df: " << _df
      << "\nkf: " << _kf
      << "\nn: " << _n
      << "\nk: " << _k
      << "\na: " << _a
      << "\n\nRg: " << _rg
      << "\n\nX Y Z" << endl;
  for (Vector_3 v : _fractal)
    out << v << endl;
}
