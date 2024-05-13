

#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Camera2D.h"
#include "Model2D.h"

class Scene2D : public Camera2D
{
private:
	typedef double (*Func)(double);
public:
	Model2D model;
	Scene2D(double L, double R, double B, double T, Model2D model) : Camera2D(L, R, B, T)
	{
		this->model = model;
	}

	void Render(HDC dc)
	{
		Axes(dc);
		for (int i = 1; i <= model.GetSize_e(); i++) {
			int
				begin = model.GetEdges()(i, 1),
				end = model.GetEdges()(i, 2);
			MoveTo(model.GetVertexX(begin), model.GetVertexY(begin));
			LineTo(dc, model.GetVertexX(end), model.GetVertexY(end));
		}
	}
};

#endif SCENE_2D_H


/*
#ifndef SCENE_2D_H
#define SCENE_2D_H
#include "Camera2D.h"
#include "Model2d.h"

Model2d AddCube();
class Scene2D : public Camera2D
{
	
public:
	Model2d obj[1] = { AddCube(0,0) };
	Model2d* selected;
	Scene2D(double L, double R, double B, double T) : Camera2D(L, R, B, T)
	{
	}
	void Render(HDC dc)
	{
		for (int i = 0; i < 1; i++)
		{
			double* arrayVertices = obj[i].GetVertexWithEdge();
			int size = obj[i].GetSizeEdge();
			for (int i = 0; i < size / 2; i++)
			{
				MoveTo(arrayVertices[i * 4 + 0], arrayVertices[i * 4 + 1]);
				LineTo(dc, arrayVertices[i * 4 + 2], arrayVertices[i * 4 + 3]);

			}
		}

	}
	void Moving(double x, double y)
	{
		if (selected == NULL)
			return;
		selected->Move(x, y);

	}
	void Rotate(double t)
	{
		if (selected == NULL)
			return;
		selected->Rotate(t);
	}
	Model2d AddCube(double x, double y)
	{
		double cubeVertex[8] = {
			5,5,
			5,-5,
			-5,-5,
			-5,5
		};
		int cubeEdge[8] = {
			0,1,
			1,2,
			2,3,
			3,0
		};
		Model2d cube(x,y);
		cube.SetVertex(cubeVertex, 8);
		cube.SetEdge(cubeEdge, 8);
		return cube;
	}
	void CheckCLick(HDC dc, int x, int y)
	{
		for (int i = 0; i < 1; i++)
		{
			if (obj[i].CheckClick(ScreenToWorldX(x), ScreenToWorldY(y)) == true)
			{
				selected = &obj[i];
			}
		}
	}
};

#endif SCENE_2D_H
*/