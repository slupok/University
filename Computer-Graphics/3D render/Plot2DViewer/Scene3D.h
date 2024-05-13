#ifndef SCENE_2D_H
#define SCENE_2D_H
#include "Camera3D.h"
#include "Model3D.h"

class Scene3D : public Camera3D
{

public:
	Model3D model;
	Scene3D(double L, double R, double B, double T, Vector3d Ov, Vector3d t, Vector3d n, double d, Model3D model) : Camera3D(L, R, B, T,Ov,t,n,d)
	{
		this->model = model;
	}

	void Render(HDC dc)
	{
		model.Project(GetMvp());
		Axes(dc);
		for (int i = 1; i <= model.GetSize_f(); i++) {

			int
				A = model.GetFaces()(i, 1),
				B = model.GetFaces()(i, 2),
				C = model.GetFaces()(i, 3);

			//AB
			MoveTo(model.GetVertexX(A), model.GetVertexY(A));
			LineTo(dc, model.GetVertexX(B), model.GetVertexY(B));

			//BC
			MoveTo(model.GetVertexX(B), model.GetVertexY(B));
			LineTo(dc, model.GetVertexX(C), model.GetVertexY(C));

			//CA
			MoveTo(model.GetVertexX(C), model.GetVertexY(C));
			LineTo(dc, model.GetVertexX(A), model.GetVertexY(A));
		}
	}
};

#endif SCENE_3D_H