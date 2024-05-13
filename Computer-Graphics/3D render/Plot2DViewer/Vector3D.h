#ifndef VECTOR_3D_H
#define VECTOR_3D_H
#include "math.h"
class Vector3d
{
private:
	double vec[3];
public:

	Vector3d()
	{
		vec[0] = 0;
		vec[1] = 0;
		vec[2] = 0;
	}
	Vector3d(double a, double b, double c)
	{
		vec[0] = a;
		vec[1] = b;
		vec[2] = c;
	}
	double& operator()(int a) { return vec[a]; }
	Vector3d operator*(const double& V)
	{
		Vector3d result;
		result.vec[0] = vec[0] * V;
		result.vec[1] = vec[1] * V;
		result.vec[2] = vec[2] * V;
		return result;
	}
	static Vector3d Cross(Vector3d a, Vector3d b)
	{
		Vector3d result;
		result.vec[0] = a.vec[1] * b.vec[2] - a.vec[2] * b.vec[1];
		result.vec[1] = a.vec[2] * b.vec[0] - a.vec[0] * b.vec[2];
		result.vec[2] = a.vec[0] * b.vec[1] - a.vec[1] * b.vec[0];
		return result;
	}
	double Magnitude()
	{
		double res = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
		return res;
	}
};

#endif VECTOR_3D_H