#pragma once
#include"Component.h"
#include"Engine/DirectX_11/Fbx.h"
class ModelComponent : public Component
{
private:
	
	Fbx* pFbxModel_;
	Transform transform_;
	std::string fileName_;

		//アニメーションのフレーム
		float nowFrame_;
		float animSpeed_;
		int	  startFrame_;
		int   endFrame_;

		void SetAnimFrame(int start, int end, float speed);
public:
	ModelComponent(std::string modelName);
	~ModelComponent();
	int GetModelHandle();
};

