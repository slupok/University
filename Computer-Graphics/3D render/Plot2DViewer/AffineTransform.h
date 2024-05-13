#include "Matrix.h"

#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

Matrix<> Translation(double x, double y, double z)
{
	double T[16] = {
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	};
	return Matrix<>(4, T);
}
Matrix<> Identity()
{
	double T[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return Matrix<>(4, T);
}
Matrix<> RotationX(double t)
{
	double T[16] = {
		1,     0,       0,  0,
		0, cos(t), -sin(t), 0,
		0, sin(t),  cos(t), 0,
		0,     0,       0,  1
	
	};
	return Matrix<>(4, T);
}
Matrix<> RotationY(double t)
{
	double T[16] = {
		cos(t),  0, sin(t),  0,
		0,       1,     0,   0,
		-sin(t), 0,  cos(t), 0,
		0,     0,       0,   1

	};
	return Matrix<>(4, T);
}
Matrix<> RotationZ(double t)
{
	double T[16] = {
		cos(t), -sin(t), 0, 0,
		sin(t),  cos(t), 0, 0,
		0,       0,      1, 0,
		0,       0,      0, 1

	};
	return Matrix<>(4, T);
}
//
Matrix<> Scaling(double kx, double ky, double kz)
{
	double T[16] = {
		kx, 0, 0, 0,
		0, ky, 0, 0,
		0, 0, kz, 0,
		0, 0, 0, 1
	};
	return Matrix<>(4, T);
}

Matrix<> MappingXY()
{
	double T[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	};
	return Matrix<>(4, T);
}
Matrix<> MappingYZ()
{
	double T[16] = {
		-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return Matrix<>(4, T);
}
Matrix<> MappingXZ()
{
	double T[16] = {
		1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return Matrix<>(4, T);
}
Matrix<> MappingX()
{
	double T[16] = {
		1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	};
	return Matrix<>(4, T);
}
Matrix<> MappingY()
{
	double T[16] = {
		-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	};
	return Matrix<>(4, T);
}
Matrix<> MappingZ()
{
	double T[16] = {
		-1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return Matrix<>(4, T);
}
Matrix<> MappingO()
{
	double T[16] = {
		-1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	};
	return Matrix<>(4, T);
}
// Здесь требуется реализовать функции, возвращающие матрицы базовых АП:
// Identity() - тождественное АП;
// Rotation(t) - поворот на угол t;
// Rotation(c, s) - поворот на угол, косинус и синус которого пропорциональны величинам c и s;
// Scaling(kx, ky) - масштабирование;
// Mapping (различные виды отражений) - по желанию, для создания матриц отражения можно использовать функцию Scaling.

// В приведённом примере используется конструктор матрицы из списка.
// Приветствуются идеи о других способах создания матриц.
// Например, вызов функции, создающей единичную матрицу,
// с последующим переопределением некоторых её коэффициентов.

#endif AFFINE_TRANSFORM_H
