#pragma once
#include<d2d1.h>
#include<dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib,"dwrite.lib")

namespace D2D
{
	extern ID2D1Factory* pFactory_;
	extern ID2D1RenderTarget* pRenderTarget_;

	HRESULT Initialize(int winW, int winH, HWND hWnd);
	void Release();
	void RenderTest();
	void Drawtext();

};

