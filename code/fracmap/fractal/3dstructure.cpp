/* 3dstrcture.cpp
 * Modified: (09/06/07) Mark Garro
 * Modified: (07/01/18) Chris Hayes
 */
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>

#include "fractal.h"
#include "settings.h"

using namespace std;

const int STRUCTURE_ITER = 500;
const int ANG_ITER = 100; // Was 2000
const int STRUCTURE_PRECISION = 1000;

/* Structure factor of the fractal */
void Fractal::structurec(ofstream & output)
{
	double qmin, qmax, qfactor, qval, Angsum;
    double Resum, Imsum;
	double dotprod;
	double Sqtotsum;
	Vector_3 qvec;

	output << "Qcount, Qval, Angsum\n";

	create_box();
	cout << _points.size() << endl;

	cout << "Vector_3 coordinates stored." << endl;

	qmin = 2 * pi / num_parts * 0.1;
	qmax = 2 * pi / (2 * _a * parts_per_radius) * 2.0;
	qfactor = pow( (qmax / qmin), (1.0 / ((double)STRUCTURE_ITER - 1.0)) );

	for(int i = 0; i < STRUCTURE_ITER; i++)
	{
		qval = qmin * pow( qfactor, i );
		Angsum = 0.0;

		for(int j = 0; j < ANG_ITER; ++j)
		{	
		    qvec = Vector_3::random_vec();
			qvec *= qval;
		  
		    Resum = 0.0;
			Imsum = 0.0;

			for(int k = 0; k < STRUCTURE_PRECISION; k++)
			{
				int rand_pt = rand() % (int)_points.size();

			    dotprod = _points[rand_pt] * qvec;
			  
				Resum += cos(dotprod);
				Imsum += sin(dotprod);
			}
			Resum *= (double) _points.size() / STRUCTURE_PRECISION;
			Imsum *= (double) _points.size() / STRUCTURE_PRECISION;

			Sqtotsum = ((Resum*Resum + Imsum*Imsum) / _points.size()*_points.size());
			Angsum  += Sqtotsum / ANG_ITER;
		}

		cout  << i + 1 << ", " << log(qval) << ", " << log(Angsum) << endl;
		output << i + 1 << ", " << log(qval) << ", " << log(Angsum) << endl;
	}
}


void Fractal::create_box()
{
	if(box_created)
		return;
	else
	{		
		Vector_3 temp;

		box_length = 0.0;

		for(vector<Vector_3>::iterator iter = _fractal.begin(); iter != _fractal.end(); iter++)
		{
			if(_r_mean.distance(*iter) > box_length) box_length = _r_mean.distance(*iter);
		}

		box_length += _a + 1.0;
		box_length *= 2;

		num_parts = (int)(box_length/_a * parts_per_radius);


		for(vector<Vector_3>::iterator iter = _fractal.begin(); iter != _fractal.end(); iter++)
		{
			for(int xcoord = 0; xcoord <= (parts_per_radius * 2 + 1); ++xcoord)
			{
				for(int ycoord = 0; ycoord <= (parts_per_radius * 2 + 1); ++ycoord)
				{
					for(int zcoord = 0; zcoord <= (parts_per_radius * 2 + 1); ++zcoord)
					{
            temp = Vector_3((double)xcoord / parts_per_radius - _a,
                            (double)ycoord / parts_per_radius - _a,
                            (double)zcoord / parts_per_radius - _a);
						temp += *iter;
						if(temp.distance(*iter) < _a)
						{	
							_points.push_back(temp);
						}
					}
				}
			}
		}
		box_created = true;
	}
}
