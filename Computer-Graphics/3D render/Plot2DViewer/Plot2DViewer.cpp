#include <windows.h>
#include <windowsx.h>
#include "Data.h"
#include "Scene3D.h"


LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// �������� ������� ���������
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// �������� ���������
{
	// ������ ������������ ����� �������� ��������� - �������� ����: ������� ����������� ������� ����� wc, ����� �������� ���� hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// ��� ������� ���������, ������������ �� ������ �������
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// ������������� ����������, ���������� ����� ������ ���������� ��� ������� ������
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// ���� � ������� ������ �����������
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// ��� �������� ������, ������������ ��� �������� ����������� ����
	RegisterClass(&wc);								// ����������� ������ wc

	HWND hWnd = CreateWindow(						// hWnd - ����������, ���������������� ����; ������� �������� ���� ��������� ���������� hWnd ��������� ���������
		(LPCSTR)"MainWindowClass",					// ��� �������� ������
		(LPCSTR)"Plot2D Viewer",					// ��������� ����
		WS_OVERLAPPEDWINDOW,						// ����� ����
		200,200,400,400,							// ���������� �� ������ ������ �������� ���� ����, ��� ������ � ������
		nullptr,nullptr,hInstance,nullptr);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// ������ ������������ ����� �������� ��������� - �������� ���� ��������� ��������� ���������, ������� ������� ��������� � ��������� �� ��������������� �����
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))				// ������� GetMessage �������� �� ������� ��������� � ������� ��� � ��������� msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// ������� DispatchMessage ��������� ������� � ������������� ������ ������� ���������
	}

	return 0;
}

// � �������� ������ ����������� ������ ���� ���������� ���������� - �������� ������ ������ Scene2D
// ��� ���������� �������� �������������� ����������� ��������� � �������, ������������� � ���� ������
Model3D model("VerticesMap.txt", "EdgesMap.txt");
Vector3d Ov(0, 0, 0);
Vector3d t(0, 1, 0);
Vector3d n(1, 1, 1);
double d = 15;
Scene3D scene(L, R, B, T, Ov, t, n, d, model);

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// ������� ��������� ��������� � ������������ ��� ���������, ������������ ����
{
	POINT P;
	int zDelta;
	char key;
	double _x, _y, _z;
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		key = wParam; 
		switch (key)
		{
		case 'W':
			scene.model.Apply(Translation(0, 0, -0.3));
			scene.model.centerZ -= 0.3;
			break;
		case 'S':
			scene.model.Apply(Translation(0, 0, 0.3));
			scene.model.centerZ += 0.3;
			break;
		case 'A':
			scene.model.Apply(Translation(-0.3, 0, 0));
			scene.model.centerX -= 0.3;
			break;
		case 'D':
			scene.model.Apply(Translation(0.3, 0, 0));
			scene.model.centerX += 0.3;
			break;

		case 'Q':
			_x = scene.model.centerX; 
			_y = scene.model.centerY;
			_z = scene.model.centerZ;
			scene.model.Apply(Translation(-_x, -_y, -_z));
			scene.model.Apply(RotationY(M_PI / 18));
			scene.model.Apply(Translation(_x, _y, _z));

			break;
		case 'E':
			_x = scene.model.centerX;
			_y = scene.model.centerY;
			_z = scene.model.centerZ;
			scene.model.Apply(Translation(-_x, -_y, -_z));
			scene.model.Apply(RotationY(-M_PI / 18));
			scene.model.Apply(Translation(_x, _y, _z));
			break;
		case 'R':
			_x = scene.model.centerX;
			_y = scene.model.centerY;
			_z = scene.model.centerZ;
			scene.model.Apply(Translation(-_x, -_y, -_z));
			scene.model.Apply(RotationZ(M_PI / 18));
			scene.model.Apply(Translation(_x, _y, _z));
			break;
		case 'T':
			_x = scene.model.centerX;
			_y = scene.model.centerY;
			_z = scene.model.centerZ;
			scene.model.Apply(Translation(-_x, -_y, -_z));
			scene.model.Apply(RotationZ(-M_PI / 18));
			scene.model.Apply(Translation(_x, _y, _z));
			break;
		case 'Y':
			_x = scene.model.centerX;
			_y = scene.model.centerY;
			_z = scene.model.centerZ;
			scene.model.Apply(Translation(-_x, -_y, -_z));
			scene.model.Apply(RotationX(M_PI / 18));
			scene.model.Apply(Translation(_x, _y, _z));
			break;
		case 'U':
			_x = scene.model.centerX;
			_y = scene.model.centerY;
			_z = scene.model.centerZ;
			scene.model.Apply(Translation(-_x, -_y, -_z));
			scene.model.Apply(RotationX(-M_PI / 18));
			scene.model.Apply(Translation(_x, _y, _z));
			break;
		}

		InvalidateRect(hWnd, nullptr, false);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{	
		if (!scene.IsDragging())
		{
			P.x = GET_X_LPARAM(lParam);
			P.y = GET_Y_LPARAM(lParam);
			ScreenToClient(hWnd, &P);
			scene.StartDragging(P.x, P.y);
		}
		return 0;
	}case WM_LBUTTONUP:
	{
		if (scene.IsDragging())
		{
			scene.StopDragging();
		}
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		if (scene.IsDragging())
		{
			P.x = GET_X_LPARAM(lParam);
			P.y = GET_Y_LPARAM(lParam);
			ScreenToClient(hWnd, &P);
			scene.Dragging(P.x, P.y);
			InvalidateRect(hWnd, nullptr, false);
		}
		return 0;
	}
	case WM_MOUSEWHEEL:
		zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		P.x = GET_X_LPARAM(lParam);
		P.y = GET_Y_LPARAM(lParam);
		ScreenToClient(hWnd, &P);
		scene.Resize(zDelta, P.x, P.y);
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	case WM_PAINT:
		{
			HDC dc = GetDC(hWnd);
			scene.Clear(dc);				// ����� �������������� � ������ Camera2D ������, ����������� �� ������� ������� ������� ���� hWnd
			//scene.Plot(dc, Sinusoid);		// ����� �������������� � ������ Scene2D ������, ����������� �� ��������� ������� ���������
			scene.Render(dc);
			ReleaseDC(hWnd,dc);
			return DefWindowProc(hWnd,msg,wParam,lParam);
		}

	case WM_SIZE:
		{
			HDC dc = GetDC(hWnd);
			scene.SetResolution(dc);
			ReleaseDC(hWnd,dc);
			InvalidateRect(hWnd,nullptr,false);
			return 0;
		}

	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

	default:
		{
			return DefWindowProc(hWnd,msg,wParam,lParam);
		}
	}
	return 0;
}
