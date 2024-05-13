#define _USE_MATH_DEFINES
#include <cmath>

double					// Исходные значения параметров L, R, B, T
	L=-3,
	R=3,
	B=-3,
	T=3;
double const C = 1;

double Parabola(double x)
{
	return x*x-2;
}

double Sinusoid(double x)
{
	return sin(x);
}
double X(double u, double v)
{
	return (C * cosh(u) * cos(v));
}
double Y(double u, double v)
{
	return (C * sinh(u) * sin(v));
}
double Ell_u(double t)
{
	return t;
}
double Ell_v(double t)
{
	return sin(t);
}
