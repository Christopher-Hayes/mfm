/* fractal.h
 * Created:  Mark Garro  (09/06/07)
 * Modified: Chris Hayes (06/26/18)
 */
#pragma once
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>

#include "log2.hpp"
#include "settings.h"
#include "Vector_3.h"

class Fractal
{
public:
  // Constructors / Deconstructors =============================================
	// Fractal(Vector_3 center);
	Fractal(double df=fractal_dimension, double kf=prefactor, double k=overlap, double e=epsilon);
	~Fractal() {}

  // Core operations ===========================================================
  void generate_fractal(int size, bool output=true);
	void monte_carlo();
  // Create/Add monomer
	void create_monomer();
	void add_monomer(double x, double y, double z);
	void add_monomer(const Vector_3& new_monomer);
	Vector_3 find_attach_monomer(const Vector_3& parent_monomer);
	Vector_3 get_random_monomer();

	bool monomer_proximity(const Vector_3& new_monomer, double threshold);
	double test_rg(const Vector_3& monomer);
	void parameter_update();

  // Getters
	double N(); // Where is this function?
	double get_df() const { return _df; }
	double get_kf() const { return _kf; }
  double e() const { return _e; }
	double radius() const {	return _a; }
	Vector_3 rmean() const { return _r_mean; }
	Vector_3 cm() const { return _r_mean; }
	double rg() const { return _rg; }
  double actual_e() const { return _sum_epsilon / _n; }
	std::vector<Vector_3>& monomers() { return _fractal;}
  
  // Vector-like operations
	unsigned int size() const { return _n; }
  void pop_back() { _fractal.pop_back(); }

  // index is returned?
  int begin() const { return 0; }
  int end() const { return _fractal.size(); }

	void clear();
  Vector_3 return_last() const { return _fractal.back(); }
  Vector_3& grab(int i) {	return _fractal[i]; }
  Vector_3 operator[](int i) { return _fractal[i]; }

  // Setters
	void set_Df(double df) { _df = df; }
	void set_kf(double kf) { _kf = kf; }

	// Structure constant functions
	void create_box();
	void structurec(std::ofstream& output);
	
	void orient_random(std::ofstream& output, unsigned int num_trials=25, bool stablize=true);
	void rotate_newZ(Vector_3 newZ);
  
	// Convex hull codes
	Vector_3& max_point(Vector_3 direction);
	Vector_3& min_angle_point(Vector_3 head, Vector_3 tail, Vector_3 dir);

	bool find_stable_vector(Vector_3 seed, Vector_3 & stableOut);
	void find_first_facet(std::vector<Vector_3>& facets, Vector_3 seed);
	void find_next_facet(std::vector<Vector_3>& facets);

	Vector_3 last_facet_normal(std::vector<Vector_3> facets);
	bool cm_proj_in_facet(std::vector<Vector_3>& facets);
	bool check_last_facet(std::vector<Vector_3>& _facets);

  // Print functions
  void print_monomers(std::ostream& out = std::cout) const;
  // void print_parameters(ostream& out = cout) const;
	
private:
  // A vector containing the centers of the monomers.
	std::vector<Vector_3> _fractal;
	
	// Fixed parameters
  double _df; // Fractal dimension
  double _kf; // Prefactor
  double _a;  // Radius?
  double _k;  // Monomer overlap factor
  double _e;  // Epsilon
  
  // Rg linear time calculations
	Vector_3 _r_sum;  
	double _r_squared;
	
  // Dependent parameters
	Vector_3 _r_mean;
	double _rg;
	int _n;

	double box_length;
	int num_parts;

  // Orientation
  std::vector<Vector_3> _facets;

  // 3D Structure
  std::vector<Vector_3> _points;

  // Analytics
  double _sum_epsilon;

  const Vector_3 center = Vector_3(0.0, 0.0, 0.0); 

	static bool last_trial; // ?
	bool box_created;
};

// ?
double psdrand(int);
