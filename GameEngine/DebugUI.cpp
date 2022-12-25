#include "DebugUI.h"
#include"Engine/SceneManager.h"
#include<iostream>
//#include<string>
namespace DebugData
{
	int objectCount_;
	struct ObjectData
	{
		std::string objectName_;
		Transform transform_;

	};

}
	

void DebugUI::Initialize(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io=ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice,pContext);

}

void DebugUI::Debug(GameObject* object)
{
	ImGui::Begin("Debug");
	ObjectCount(object);
	std::string count = std::to_string(DebugData::objectCount_);
	ImGui::Text(count.c_str());
	ImGui::Text(object->GetObjectName().c_str());
	DebugData::objectCount_ = 0;
	ImGui::End();
	
}

void DebugUI::StartImGui()
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
}

void DebugUI::CleanUp()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugUI::ObjectCount(GameObject* object)
{
	if (object == nullptr)
	{
		return;
	}
	DebugData::objectCount_++;
	if (ImGui::TreeNode(object->GetObjectName().c_str()))
	{

		ImGui::TreePop();
	}

	for (auto itr = object->GetChildList()->begin(); itr != object->GetChildList()->end(); itr++)
	{
		ObjectCount((*itr));
	}
}
