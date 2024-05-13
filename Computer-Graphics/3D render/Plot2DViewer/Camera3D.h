#ifndef CAMERA_3D_H
#define CAMERA_3D_H
#include "Camera2D.h"
#include "Vector3D.h"
#include "Matrix.h"
class Camera3D : public Camera2D
{
protected:
	Vector3d Ov,T,N;
	double D;
	Matrix<> Mview;
	Matrix<> Mproj;
	Matrix<> Mvp;
	Matrix<> WorldToView()
	{
		Vector3d k = N * (1/N.Magnitude());
		Vector3d i = Vector3d::Cross(T, N) * (1 / (Vector3d::Cross(T, N)).Magnitude());
		Vector3d j = Vector3d::Cross(k, i);
		double T[16] = {
			i(0), i(1), i(2), -(i(0) * Ov(0) + i(1) * Ov(1) + i(2) * Ov(2)),
			j(0), j(1), j(2), -(j(0) * Ov(0) + j(1) * Ov(1) + j(2) * Ov(2)),
			k(0), k(1), k(2), -(k(0) * Ov(0) + k(1) * Ov(1) + k(2) * Ov(2)),
			  0,    0,    0,                        1
		};
		return Matrix<>(4, T);
	}
	Matrix<> ViewToProj()
	{
		double T[12] = {
			1, 0,     0,    0,
			0, 1,     0,    0,
			0, 0, -(1 / D), 1
		};
		return Matrix<>(3, 4, T);
	}
	Matrix<> WorldToProj()
	{
		Matrix<> tmp =  Mproj * Mview;
		return tmp;
	}
	void SetCamera(Vector3d o, Vector3d t, Vector3d n, double d)
	{
		Ov = o;
		T = t;
		N = n;
		D = d;
		UpdateCamera();
	}
	void UpdateCamera()
	{
		Mview = WorldToView();
		Mproj = ViewToProj();
		Mvp = WorldToProj();
	}
public:
	Camera3D(double L, double R, double B, double T, Vector3d Ov, Vector3d t, Vector3d n, double d) :
		Camera2D(L, R, B, T)
	{
		SetCamera(Ov, t, n, d);
	}
	Matrix<> GetMview()
	{
		return Mview;
	}
	Matrix<> GetMproj()
	{
		return Mproj;
	}
	Matrix<> GetMvp()
	{
		return Mvp;
	}
};

#endif CAMERA_3D_H