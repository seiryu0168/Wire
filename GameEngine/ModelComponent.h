#pragma once
#include"Component.h"
#include"Engine/DirectX_11/Fbx.h"
class ModelComponent : public Component
{
private:

	Fbx* pFbxModel_;
	Transform transform_;
	std::string fileName_;
	SHADER_TYPE shaderType_;
	//アニメーションのフレーム
	float nowFrame_;
	float animSpeed_;
	int	  startFrame_;
	int   endFrame_;

	void SetAnimFrame(int start, int end, float speed);
public:
	ModelComponent(std::string modelName, GameObject* obj);
	~ModelComponent();
	void Load(std::string fileName);
	void SetTransform(const Transform* transform=nullptr);
	void SetShader(SHADER_TYPE type);
	void Update(bool active) override;
	XMFLOAT3 GetBonPosition(std::string boneName);
	void Draw();
	void Draw(const Transform* transform);
};