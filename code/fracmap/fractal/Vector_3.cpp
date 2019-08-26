/* Vector_3.cpp
 * Modified: Mark Garro (09/06/07)
 * Modified: Chris Hayes (06/29/18)
 */
#include "settings.h"
#include "Vector_3.h"

using namespace std;

double tri_area(Vector_3 pt1, Vector_3 pt2, Vector_3 pt3) {
  Vector_3 a = pt2 - pt1;
  Vector_3 b = pt3 - pt1;
	
  return 0.5 * a.X(b).magnitude();
}
void Vector_3::
random_quat(double Quat[]) {
  double u1 = (double)rand() / RAND_MAX;
  double u2 = (double)rand() / RAND_MAX;
  double u3 = (double)rand() / RAND_MAX;
	
  Quat[0] = sqrt(1-u1) * sin(2*pi*u2);
  Quat[1] = sqrt(1-u1) * cos(2*pi*u2);
  Quat[2] = sqrt(u1) * sin(2*pi*u3);
  Quat[3] = sqrt(u1) * cos(2*pi*u3);
}

Vector_3 Vector_3::
random_vec() {
  double u1 = (double)rand() / RAND_MAX;
  double u2 = (double)rand() / RAND_MAX;
  return Vector_3(sqrt(4*u2 - 4*u2*u2) * sin(2*pi*u1),
		              sqrt(4*u2 - 4*u2*u2) * cos(2*pi*u1),
		              (2*u2 - 1));
}

void QuatToMatrix( const double Quat[], double Matrix[] ) {
  double w = Quat[0];
  double i = Quat[1];
  double j = Quat[2];
  double k = Quat[3];
	
  Matrix[0] = w*w + i*i - j*j - k*k;
  Matrix[1] = 2*i*i - 2*w*k;
  Matrix[2] = 2*w*j + 2*i*k;
	
  Matrix[0+3] = 2*w*k + 2*i*j;
  Matrix[1+3] = w*w - i*i + j*j - k*k;
  Matrix[2+3] = 2*j*k - 2*w*i;
	
  Matrix[0+6] = 2*i*k - 2*w*i;
  Matrix[1+6] = 2*w*i + 2*i*k;
  Matrix[2+6] = w*w - i*i - j*j + k*k;
}
// TODO: fix this mess
void VectorToMatrix( const Vector_3 newZ, double RotMatrix[] )
{
  //TODO: Convert the newZ to a rotation matrix.
  
  Vector_3 zAxis = Vector_3(0,0,1);
  Vector_3 axis = newZ.X(zAxis).normalize();
  double sina = newZ.X(zAxis).magnitude(); //Cross produce = |a||b|sin alpha
  double cosa = newZ * zAxis; // Dot producct = |a||b|cos alpha
  double a = atan2( sina, cosa );
  
  // w i j k
  double rot_quat[4] = {cos(a/2),
                       axis.x * sin(a/2),
                       axis.y * sin(a/2),
                       axis.z * sin(a/2)};
  double mag = rot_quat[0] * rot_quat[0] +
               rot_quat[1] * rot_quat[1] +
               rot_quat[2] * rot_quat[2] +
               rot_quat[3] * rot_quat[3];
  
  //cout << "Angle a: " << a << endl;
  //cout << "Quat magnitude: " << mag << endl;
  
  if( fabs( mag - 1 ) < .05 ) //The Quaternion was well defined
  {
	//Cheating and reusing code.
	//It's easy to express the rotation we need with Quaternions, and we can then convert it to a matrix.
	
	QuatToMatrix( rot_quat, RotMatrix );
  } else { //The rotation was too parallel to the Z axis already. 
	//Return +/- identity matrix.
	for( int i = 0; i< 9; i++ )
	  RotMatrix[i] = 0;
	
	RotMatrix[0] = RotMatrix[4] = RotMatrix[8] = cosa < 0 ? -1 : 1; //flip depending on projection of newZ on oldZ
  }
}

double Vector_3::magnitude() const
{
  return sqrt( x*x + y*y + z*z ) ;
}

Vector_3 Vector_3::normalize() const
{
  double norm = magnitude();
  return Vector_3(x/norm,y/norm,z/norm);
}

double Vector_3::distance(Vector_3 pt2) const
{
  return sqrt(square_difference(pt2));
}

double Vector_3::square_difference(Vector_3 pt2) const
{
  double x1 = x - pt2.x,
    y1 = y - pt2.y,
    z1 = z - pt2.z;
	
  return (x1*x1 + y1*y1 + z1*z1);
}

// X?
Vector_3 Vector_3::
X(const Vector_3 rhs) const {
  return Vector_3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
}

// Print -----------------------------------------------------------------------
string Vector_3::
print() const {
  return "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
}

// Vector overloads ============================================================
// Vector_3 * Vector_3 ---------------------------------------------------------
double Vector_3::
operator*(const Vector_3 rhs) const {
  return x*rhs.x + y*rhs.y + z*rhs.z;
}
// Vector_3 + Vector_3 ---------------------------------------------------------
Vector_3 Vector_3::
operator+(const Vector_3& rhs) const {
  Vector_3 temp(x + rhs.x,
		            y + rhs.y,
		            z + rhs.z);
  return temp;
}
// Vector_3 - Vector_3 ---------------------------------------------------------
Vector_3 Vector_3::
operator-(const Vector_3& rhs) const {
  Vector_3 temp(x - rhs.x,
		            y - rhs.y,
		            z - rhs.z);
  return temp;
}
// Vector_3 += Vector_3 --------------------------------------------------------
Vector_3& Vector_3::
operator+=(const Vector_3 & rhs) {
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  return *this;
}
// Vector_3 -= Vector_3 --------------------------------------------------------
Vector_3& Vector_3::
operator-=(const Vector_3 & rhs) {
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  return *this;
}

// Scalar overloads ============================================================
// Vector_3 / double -----------------------------------------------------------
Vector_3 Vector_3::
operator/(const double& div) const {
  if (div == 0) {
    WARN << "Dividing vector by zero.";
    return *this;
  }
  return Vector_3(x / div, y / div, z / div);
}
// Vector_3 * double -----------------------------------------------------------
Vector_3 Vector_3::
operator*(const double& scalar) const {
  return Vector_3(x*scalar, y*scalar, z*scalar);
}
// Vector_3 / double -----------------------------------------------------------
Vector_3 & Vector_3::
operator/=(const double& scalar) {
  x /= scalar;
  y /= scalar;
  z /= scalar;
  return *this;
}
// Vector_3 *= double ----------------------------------------------------------
Vector_3 & Vector_3::
operator*=(const double& scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
  return *this;
}
// Vector_3 = Vector_3 ---------------------------------------------------------
Vector_3 & Vector_3::
operator=(const Vector_3 & rhs) {
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  return *this;
}
// Vector_3 == Vector_3 --------------------------------------------------------
bool Vector_3::
operator==( const Vector_3 & b ) const {
  return x == b.x && y == b.y && z == b.z;
}
