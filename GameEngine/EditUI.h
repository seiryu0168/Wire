#pragma once
#include"ImGui/imgui_impl_dx11.h"
#include"ImGui/imgui_impl_win32.h"
#include"Engine/Direct3D.h"
namespace EditUI
{
	void Initialize(HWND hWnd,ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	void StartImGui();
	void UpDate();
	void CleanUp();
};

