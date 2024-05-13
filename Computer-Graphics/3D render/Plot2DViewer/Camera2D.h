#ifndef CAMERA_2D_H
#define CAMERA_2D_H
#include <windows.h>

class Camera2D
{
protected:
	bool isDragging;
	int previousX, previousY;
	double L, R, B, T;					// Мировые координаты границ рабочей области окна
	int W, H;							// Разрешение рабочей области окна
	int WorldToScreenX(double X)		// Переход от мировых координат к экранным (для абсциссы)
	{
		int Xs = (int)((X - L) / (R - L) * W);
		return Xs;
	}
	int WorldToScreenY(double Y)		// Переход от мировых координат к экранным (для ординаты)
	{
		int Ys = (int)((T - Y) / (T - B) * H);
		return Ys;
	}
	double ScreenToWorldX(int X)		// Переход от экранных координат к мировым (для абсциссы)
	{
		double Xw = L + (R - L) * (X / (double)W);
		return Xw;
	}
	double ScreenToWorldY(int Y)		// Переход от экранных координат к мировым (для ординаты)
	{
		double Yw = T - (T - B) * (Y / (double)H);
		return Yw;
	}
private:
	double posX, posY;					// Позиция графического курсора в мировых координатах (для функций MoveTo и LineTo)
public:
	Camera2D(double L, double R, double B, double T) : L(L), R(R), B(B), T(T)
	{

	}
	void Clear(HDC dc)
	{
		HBRUSH color = CreateSolidBrush(RGB(200, 255, 255));
		// Очистка рабочей области окна
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		FillRect(dc, &r, color);

	}
	bool IsDragging()
	{
		return isDragging;
	}
	void StartDragging(int x, int y)
	{
		previousX = x;
		previousY = y;
		isDragging = true;
	}
	void StopDragging()
	{
		isDragging = false;
	}
	void Dragging(int x, int y)
	{
		int newX = x;
		int newY = y;
		double dx = ((double)newX - previousX);
		double dy = ((double)previousY - newY);
		double _L = L - (R - L) * (dx / W);
		double _R = R - (R - L) * (dx / W);
		double _B = B - (T - B) * (dy / H);
		double _T = T - (T - B) * (dy / H);
		L = _L;
		R = _R;
		B = _B;
		T = _T;
		previousX = newX;
		previousY = newY;
	}

	void Resize(int scroll, int x, int y)
	{
		double k = 1;
		if(scroll > 0)
			k = 1.1;
		if (scroll < 0)
			k = 0.9;

		L = ScreenToWorldX(x) - (ScreenToWorldX(x) - L) / k;
		R = ScreenToWorldX(x) + (R - ScreenToWorldX(x)) / k;
		B = ScreenToWorldY(y) - (ScreenToWorldY(y) - B) / k;
		T = ScreenToWorldY(y) + (T - ScreenToWorldY(y)) / k;
	}
	void SetResolution(HDC dc)
	{
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		
		W = r.right + 1;
		H = r.bottom + 1;
	
		double _L = ((L + R) / 2) - ((T - B) / 2) * ((double)W / H);
		double _R = ((L + R) / 2) + ((T - B) / 2) * ((double)W / H);
		L = _L;
		R = _R;

		// Данная процедура вызывается при изменении размеров окна
		// В ней задаются значения величин W, H, а также настраиваются значения параметров L, R, B, T таким образом, чтобы обеспечить одинаковые масштабы по координатным осям
	}
	void MoveTo(double X, double Y)
	{
		// Перемещение графического курсора (posX, posY)
		// Обратите внимание, что мы действуем в мировых координатах
		posX = X;
		posY = Y;
	}
	void LineTo(HDC dc, double X, double Y)
	{
		// Отрисовка линии из текущей позиции графического курсора в точку (X, Y) и его перемещение в эту точку
		// Обратите внимание, что мы действуем в мировых координатах
		// При отрисовке линии могут быть использованы WinApi функции
		// ::MoveToEx(dc, Xs, Ys, nullptr) и ::LineTo(dc, Xs, Ys)
		MoveToEx(dc,WorldToScreenX(posX),WorldToScreenY(posY), nullptr);
		::LineTo(dc,WorldToScreenX(X),WorldToScreenY(Y));
		MoveTo(X,Y);
	}
	void Axes(HDC dc)
	{
		HPEN pen;
		// Отрисовка координатных осей
		pen = (HPEN)SelectObject(dc, CreatePen(PS_SOLID, 1, RGB(255, 0, 0)));
		MoveTo(L, 0);
		LineTo(dc, R, 0);

		pen = (HPEN)SelectObject(dc, CreatePen(PS_SOLID, 1, RGB(0, 0, 255)));
		MoveTo(0, B);
		LineTo(dc, 0, T);


		pen = (HPEN)SelectObject(dc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0)));
	
	}
	void AxesElliptic(HDC dc)
	{
		HPEN pen;
		double step = 2 * M_PI / 36;

		pen = (HPEN)SelectObject(dc, CreatePen(PS_SOLID, 1, RGB(255, 0, 0)));
		for (double v = 0; v < 2 * M_PI; v += step)
		{
			MoveTo(X(0, v), Y(0, v));
			for (double u = 0 + step; u < 5; u += 0.1)
			{
				LineTo(dc, X(u, v), Y(u, v));
			}
		}

		pen = (HPEN)SelectObject(dc, CreatePen(PS_SOLID, 1, RGB(0, 0, 255)));
		for (double u = 0 + step; u < 5; u += 0.1)
		{
			MoveTo(X(u, 0), Y(u, 0));
			for (double v = 0; v < 2 * M_PI; v += step)
			{
				//MoveTo(x(0, v), y(0, v));

				LineTo(dc, X(u, v), Y(u, v));
			}
		}
		pen = (HPEN)SelectObject(dc, CreatePen(PS_SOLID, 2, RGB(0, 255, 0)));
	}
};

#endif CAMERA_2D_H
