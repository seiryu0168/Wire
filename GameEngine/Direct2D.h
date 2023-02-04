#pragma once
#include<d2d1.h>

#pragma comment(lib, "d2d1.lib")
namespace D2D
{
	extern ID2D1Factory* pFactory_;
	extern ID2D1HwndRenderTarget* pRenderTarget_;
	HRESULT Initialize(int winW, int winH, HWND hWnd);
	void Release();
	void RenderTest();
};

