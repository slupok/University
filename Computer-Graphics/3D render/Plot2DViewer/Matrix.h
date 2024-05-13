#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

using namespace std;


template <typename Cell = double>
class Matrix
{
private:
	int rows, cols;
	Cell** cells;
	void AllocateCells(int, int);
	void FreeCells();
public:
	Matrix() : rows(0), cols(0), cells(nullptr) {}	// Конструктор по умолчанию
	Matrix(const Matrix&);					// Конструктор копирования
	Matrix(int, int);							// Конструктор нулевой матрицы
	Matrix(int);
	Matrix(int, int, Cell*);						// Конструктор матрицы из списка
	Matrix(int, Cell*);
	~Matrix();								// Деструктор

	Cell& operator()(int i, int j) { return cells[i - 1][j - 1]; }
public:
	int GetCols() const { return cols; }
	int GetRows() const { return rows; }

	Matrix& operator = (const Matrix&);		// Перегрузка оператора присваивания
	Matrix  operator + (const Matrix&);		// Сложение матриц
	Matrix  operator - (const Matrix&);		// Вычитание матриц
	Matrix  operator * (const Matrix&);		// Умножение матриц

	friend istream& operator >> <> (istream&, Matrix&);			// Перегрузка оператора >> для ввода матрицы
	friend ostream& operator << <> (ostream&, const Matrix&);	// Перегрузка оператора << для вывода матрицы
};


template <typename Cell>
Matrix<Cell>::Matrix(const Matrix<Cell>& M)
{
	AllocateCells(M.rows, M.cols);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = M.cells[i][j];
}

template <typename Cell>
Matrix<Cell>::Matrix(int Rows, int Cols)
{
	AllocateCells(Rows, Cols);
	for (int i = 0; i < Rows; i++)
		for (int j = 0; j < Cols; j++)
			cells[i][j] = 0;
}

template<typename Cell>
inline Matrix<Cell>::Matrix(int size)
{
	AllocateCells(size, size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			cells[i][j] = 0;
}

template <typename Cell>
Matrix<Cell>::Matrix(int Rows, int Cols, Cell* list)
{
	AllocateCells(Rows, Cols);
	for (int i = 0; i < Rows; i++)
		for (int j = 0; j < Cols; j++)
			cells[i][j] = list[i * Cols + j];
}

template<typename Cell>
inline Matrix<Cell>::Matrix(int size, Cell* list)
{
	AllocateCells(size, size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			cells[i][j] = list[i * size + j];
}

template <typename Cell>
Matrix<Cell>::~Matrix()
{
	FreeCells();
}

template <typename Cell>
Matrix<Cell>& Matrix<Cell>::operator=(const Matrix& M)
{
	if (rows != M.rows || cols != M.cols)
	{
		FreeCells();
		AllocateCells(M.rows, M.cols);
	}
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = M.cells[i][j];
	return *this;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator+(const Matrix& M)
{
	Matrix<Cell> res(*this);
	if (rows == M.rows && cols == M.cols)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				res.cells[i][j] += M.cells[i][j];
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator-(const Matrix& M)
{
	Matrix<Cell> res(*this);
	if (rows == M.rows && cols == M.cols)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				res.cells[i][j] -= M.cells[i][j];
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator*(const Matrix& M)
{
	Matrix<Cell> res(rows, M.cols);
	if (cols == M.rows) {
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < M.cols; j++) {
				Cell S(0);
				for (int k = 0; k < cols; k++)
					S += cells[i][k] * M.cells[k][j];
				res.cells[i][j] = S;
			}
	}
	return res;
}

template <typename Cell>
void Matrix<Cell>::AllocateCells(int Rows, int Cols)
{
	cells = new Cell * [Rows];
	for (int i = 0; i < Rows; i++)
		cells[i] = new Cell[Cols];
	rows = Rows;
	cols = Cols;
}

template <typename Cell>
void Matrix<Cell>::FreeCells()
{
	for (int i = 0; i < rows; i++)
		delete cells[i];
	delete cells;
	rows = cols = 0;
}

template <typename Cell>
istream& operator >> (istream& fi, Matrix<Cell>& M)
{
	for (int i = 0; i < M.rows; i++)
		for (int j = 0; j < M.cols; j++)
			fi >> M.cells[i][j];
	return fi;
}

template <typename Cell>
ostream& operator << (ostream& fo, const Matrix<Cell>& M)
{
	for (int i = 0; i < M.rows; i++)
	{
		fo << "  ";
		for (int j = 0; j < M.cols; j++)
			fo << M.cells[i][j] << " \t";
		fo << endl;
	}
	return fo;
}

#endif MATRIX_H
