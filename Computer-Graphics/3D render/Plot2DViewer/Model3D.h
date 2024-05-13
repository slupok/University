#ifndef MODEL_2D_H
#define MODEL_2D_H

#include <string>
#include "fstream"
#include "Matrix.h"
#include "AffineTransform.h"

class Model3D
{
private:
	int size_v, size_f;
	Matrix<> m_Vertices;
	Matrix<int> m_Faces;
	Matrix<> CumulativeAT = Identity();
	Matrix<> m_BaseVertices;
	Matrix<> p_Vertices;
public:
	Model3D() : m_Vertices(), m_Faces(), m_BaseVertices(), CumulativeAT(Identity()), size_v(0), size_f(0) {}
	Model3D(const Matrix<> Vertices, const Matrix<int> Faces) :
		m_Vertices(Vertices), m_Faces(Faces), m_BaseVertices(Vertices), CumulativeAT(Identity()), size_v(Vertices.GetCols()), size_f(Faces.GetRows()) {}
	Model3D(string, string);
	Matrix<> GetVertices() { return m_Vertices; }
	Matrix<int> GetFaces() { return m_Faces; }
	double GetVertexX(int);
	double GetVertexY(int);
	//double GetVertexZ(int);
	void Apply(Matrix<> AT);
	void Project(Matrix<> P);
	int GetSize_v() { return size_v; }
	int GetSize_f() { return size_f; }
	double centerX = 0, centerY = 0, centerZ = 0;

};

Model3D::Model3D(string VerticesFile, string EdgesFile)
{
	size_v = 0;
	ifstream VerticesMap(VerticesFile);
	VerticesMap >> size_v;
	m_Vertices = Matrix<>(4, size_v);
	VerticesMap >> m_Vertices;
	VerticesMap.close();
	size_f = 0;
	ifstream EdgesMap(EdgesFile);
	EdgesMap >> size_f;
	m_Faces = Matrix<int>(size_f, 3);
	EdgesMap >> m_Faces;
	EdgesMap.close();
	m_BaseVertices = m_Vertices;
	//CumulativeAT = Identity();
}

double Model3D::GetVertexX(int i)
{
	return p_Vertices(1, i) / p_Vertices(3, i);
}

double Model3D::GetVertexY(int i)
{
	return p_Vertices(2, i) / p_Vertices(3, i);
}

void Model3D::Apply(Matrix<> AT)
{
	CumulativeAT = AT * CumulativeAT;
	m_Vertices = CumulativeAT * m_BaseVertices;
}
void Model3D::Project(Matrix<> P)
{
	p_Vertices = P * m_Vertices;
}
#endif MODEL_2D_H