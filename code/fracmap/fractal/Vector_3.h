/* Vector_3.h
 * Created:  (02/13/09) Shammah Chancellor. Copyright 2009 University of Nevada, Reno. All rights reserved.
 * Modified: (06/26/18) Chris Hayes
 */
#pragma once
#include <string>
#include "log2.hpp"
#include "settings.h"

class Vector_3
{
public:
  // Public members
	double x, y, z;
  // Constructors / Deconstructors
	Vector_3(double p_x=0, double p_y=0, double p_z=0): x(p_x), y(p_y), z(p_z) {}
	~Vector_3() {}
	// Calculations
	double distance(Vector_3 pt2) const;	
	double square_difference(Vector_3 target) const;
	double magnitude() const;
	Vector_3 normalize() const;
  std::string print() const;
  // Operator overloads
  // TODO: consider making these inline
	Vector_3 operator-(const Vector_3& rhs) const;
	Vector_3 operator+(const Vector_3& rhs) const;
	Vector_3 operator/(const double& den) const;
	Vector_3 operator*(const double& mult) const;
	
	double operator*(const Vector_3 rhs ) const;
	Vector_3 X(const Vector_3 rhs ) const;
	
	Vector_3& operator-=(const Vector_3& rhs);
	Vector_3& operator+=(const Vector_3& rhs);
	Vector_3& operator/=(const double& div);
	Vector_3& operator*=(const double& mult);
	
	Vector_3& operator=(const Vector_3& o);
	bool operator==(const Vector_3& b) const;

  // Static utility functions
  static void random_quat(double Quat[]);
  static Vector_3 random_vec();
};

inline std::ostream& operator<<(std::ostream& out, const Vector_3& v) {
  return out << v.x << " " << v.y << " " << v.z;
}

void QuatToMatrix( const double Quat[], double Matrix[] );  // 4 and 9 respectively
void VectorToMatrix( const Vector_3 newZ, double RotMatrix[] );

// ?
template<class T1, class T2, class T3>
double tri_area(T1 pt1, T2 pt2, T3 pt3) {
	double a = pt1.x - pt3.x;
	double b = pt1.y - pt3.y;
	double c = pt2.x - pt3.x;
	double d = pt2.y - pt3.y;
	return (0.5 * fabs((a * d) - (b * c)));
}
