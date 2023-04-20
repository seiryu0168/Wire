#pragma once
#include"Component.h"
#include"Engine/DirectX_11/Fbx.h"
class ModelComponent : public Component
{
private:
	int hModel_;

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
	ModelComponent(GameObject* obj);
	ModelComponent(std::string modelName, GameObject* obj);
	~ModelComponent();
	void Load(std::string fileName);
	void SetTransform(const Transform* transform=nullptr);
	void SetShader(SHADER_TYPE type);
	void Update(bool active) override;
	Transform GetTransform() { return transform_; }
	Fbx* GetFbxModel() { return pFbxModel_; }
	int GetModelHandle() { return hModel_; }
	XMFLOAT3 GetBonPosition(std::string boneName);
	void Draw();
	void Draw(const Transform* transform);
	void Release();

	void RayCast(ModelComponent* mComp,RayCastData& rayData);
};