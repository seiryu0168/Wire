#pragma once

#include <dInput.h>
#include"Direct3D.h"
#include"XInput.h"
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")
#pragma comment(lib,"Xinput.lib")
#include"SAFE_DELETE_RELEASE.h"

namespace Input
{
	void Initialize(HWND hWnd);
	void Update();
	//キーボード
	bool IsKey(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsKeyUp(int keyCode);

	//マウス
	bool IsMouseButton(int buttonCode);
	bool IsMouseButtonDown(int buttonCode);
	bool IsMouseButtonUp(int buttonCode);

	//コントローラー
	bool IsPadButton(int buttonCode);
	bool IsPadButtonDown(int buttonCode);
	bool IsPadButtonUp(int buttonCode);

	XMVECTOR GetMousePosition();
	void SetMousePosition(int x, int y);
	void Release();
};