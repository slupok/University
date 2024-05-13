
#ifndef MODEL_2D_H
#define MODEL_2D_H
#include <string>
#include "fstream"
#include "Matrix.h"
#include "AffineTransform.h"

class Model2D
{
private:
	int size_v, size_e;
	Matrix<> m_Vertices;
	Matrix<int> m_Edges;
	Matrix<> CumulativeAT = Identity();
	Matrix<> m_BaseVertices;
public:
	Model2D() : m_Vertices(), m_Edges(), m_BaseVertices(), CumulativeAT(Identity()), size_v(0), size_e(0) {}
	Model2D(const Matrix<> Vertices, const Matrix<int> Edges) :
		m_Vertices(Vertices), m_Edges(Edges), m_BaseVertices(Vertices), CumulativeAT(Identity()), size_v(Vertices.GetCols()), size_e(Edges.GetRows()) {}
	Model2D(string, string);
	Matrix<> GetVertices() { return m_Vertices; }
	Matrix<int> GetEdges() { return m_Edges; }
	double GetVertexX(int);
	double GetVertexY(int);
	void Apply(Matrix<>);
	int GetSize_v() { return size_v; }
	int GetSize_e() { return size_e; }
	double centerX = 0, centerY = 0;
};

Model2D::Model2D(string VerticesFile, string EdgesFile)
{
	size_v = 0;
	double temp;
	ifstream VerticesMap(VerticesFile);
	while (!VerticesMap.eof()) {
		VerticesMap >> temp;
		size_v++;
	}
	size_v /= 3;
	m_Vertices = Matrix<>(3, size_v);
	VerticesMap.close();
	VerticesMap.open(VerticesFile);
	VerticesMap >> m_Vertices;
	VerticesMap.close();
	size_e = 0;
	ifstream EdgesMap(EdgesFile);
	while (!EdgesMap.eof()) {
		EdgesMap >> temp;
		size_e++;
	}
	size_e /= 2;
	m_Edges = Matrix<int>(size_e, 2);
	EdgesMap.close();
	EdgesMap.open(EdgesFile);
	EdgesMap >> m_Edges;
	EdgesMap.close();
	m_BaseVertices = m_Vertices;
	//CumulativeAT = Identity();
}

double Model2D::GetVertexX(int i)
{
	return m_Vertices(1, i) / m_Vertices(3, i);
}

double Model2D::GetVertexY(int i)
{
	return m_Vertices(2, i) / m_Vertices(3, i);
}
void Model2D::Apply(Matrix<> AT)
{
	CumulativeAT = AT * CumulativeAT;
	m_Vertices = CumulativeAT * m_BaseVertices;
}
#endif MODEL_2D_H
/*#pragma once
#include "AffineTransform.h";
#include "Matrix.h";

double* GetVertexWithEdge();
class Model2d
{
private:
	int v_size;
	int e_size;
	double *m_vertices;//[x1,y1,x2,y2,....,xn,yn]
	int* m_edge;

	//определение ориентации точки C, относительно отрезка AB
	// > 0 - слева
	// < 0 - справа
	// == 0 - на одной прямой()
	double pointOrientation(double* A, double* B, double* C)
	{
		return (B[0] - A[0]) * (C[1] - B[1]) - (B[1] - A[1]) * (C[0] - B[0]);
	}
	bool intersect(double* A, double* B, double* C, double* D)
	{
		return pointOrientation(A, B, C) * pointOrientation(A, B, D) <= 0 && pointOrientation(C, D, A) * pointOrientation(C, D, B) < 0;
	}
public:
	Model2d() {}
	Model2d(double x, double y) : centerX(x), centerY(y) {}
	double centerX = 0;
	double centerY = 0;
	//лучевой тест ориентации точки
	bool CheckClick(double x, double y)
	{
		int k = 0;
		double* vertices = GetVertexWithEdge();
		double A[2],B[2],C[2],D[2];
		C[0] = x; C[1] = y;
		D[0] = x + 100; D[1] = y;
		for (int i = 0; i < e_size / 2; i++)
		{
			A[0] = vertices[i * 4 + 0];
			A[1] = vertices[i * 4 + 1];
			B[0] = vertices[i * 4 + 2];
			B[1] = vertices[i * 4 + 3];

			if (intersect(A, B, C, D) == true)
				k++;

		}
		if (k % 2 == 1)
			return true;
		else
			return false;
	}
	void SetPosition(double x, double y)
	{
		centerX = x;
		centerY = y;
		//SetVertex(m_vertices, v_size);
	}
	void SetVertex(double* vertex, int size)
	{
		v_size = size;
		m_vertices = new double[v_size];
		for (int i = 0; i < v_size/2; i++)
		{
			m_vertices[i * 2 + 0] = vertex[i * 2 + 0];
			m_vertices[i * 2 + 1] = vertex[i * 2 + 1];
			//m_vertices[i * 3 + 2] = 0;
		}
	}
	void SetEdge(int* edge, int size)
	{
		e_size = size;
		m_edge = new int[size];
		for (int i = 0; i < size; i++)
		{
			m_edge[i] = edge[i];
		}
	}
	int GetSizeEdge()
	{
		return e_size;
	}
	//возвращает последовательность всех пар вершин, соедененные ребром
	//(с учетом позиции центра координат)
	//записанны в одномерном массиве, чредуются пары каждые 4 элемента в массиве:
	//{ x1,y1,x2,y2 | x1,y1,x3,y3 | ...}
	double* GetVertexWithEdge()
	{
		double* vertices = new double[e_size];
		for (int i = 0; i < e_size/2; i++)
		{

			vertices[i * 4 + 0] = m_vertices[m_edge[i * 2 + 0]*2 + 0] + centerX;
			vertices[i * 4 + 1] = m_vertices[m_edge[i * 2 + 0]*2 + 1] + centerY;
			vertices[i * 4 + 2] = m_vertices[m_edge[i * 2 + 1]*2 + 0] + centerX;
			vertices[i * 4 + 3] = m_vertices[m_edge[i * 2 + 1]*2 + 1] + centerY;
		}
		return vertices;
	}
	void Move(double x, double y)
	{
		SetPosition(centerX + x, centerY + y);
	}
	void Rotate(double t)
	{
		Matrix<> r = Rotation(t);
		for (int i = 0; i < v_size / 2; i++)
		{
			//x = x*cos(t) + y*(-sin(t))
			//y = x*sin(t) + y*cos(t)
			double x = m_vertices[i * 2 + 0];
			double y = m_vertices[i * 2 + 1];

			m_vertices[i * 2 + 0] = x * r(1, 1) + y * r(1, 2);
			m_vertices[i * 2 + 1] = x * r(2, 1) + y * r(2, 2);
		}
	}
};*/