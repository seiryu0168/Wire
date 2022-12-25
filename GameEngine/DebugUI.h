#pragma once
#include"ImGui/imgui_impl_dx11.h"
#include"ImGui/imgui_impl_win32.h"
#include"Engine/Direct3D.h"

class GameObject;
namespace DebugUI
{
	void Initialize(HWND hWnd,ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	void StartImGui();
	void Debug(GameObject* object);
	void CleanUp();

	void ObjectCount(GameObject* object);
};

