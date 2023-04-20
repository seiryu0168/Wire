#include "ModelComponent.h"
#include"Engine/ResourceManager/Model.h"


ModelComponent::ModelComponent(GameObject* obj)
	:Component(obj)
{
}

ModelComponent::ModelComponent(std::string modelName,GameObject* obj)
	:Component(obj)
{
	pFbxModel_ = nullptr;
	nowFrame_ = 0;
	animSpeed_ = 0;
	startFrame_ = 0;
	endFrame_ = 0;
	shaderType_ = SHADER_TYPE::SHADER_3D;
	Load(modelName);
}

ModelComponent::~ModelComponent()
{
}

void ModelComponent::Load(std::string fileName)
{
	//ModelManagerに同一モデルの有無を確認して、モデルがあった場合は
	//そのモデルのポインタを格納
	//なかった場合は読み込む
	pFbxModel_ = ModelManager::LoadModel(fileName);
	hModel_ = ModelManager::AddMComponentList(this);
}

void ModelComponent::SetTransform(const Transform* transform)
{
	//transformがnullptrだったら
	if (transform == nullptr)
	{
		transform_ = GetAttachObject()->GetTransform();
		return;
	}

		transform_ = *transform;
}

void ModelComponent::SetShader(SHADER_TYPE type)
{
	shaderType_ = type;
}

void ModelComponent::Update(bool active)
{
	//コンポーネントがアクティブなら
	if (IsActive())
	{
		Draw();
	}
}

XMFLOAT3 ModelComponent::GetBonPosition(std::string boneName)
{
	//ボーンの位置を取得、変形して返す
	XMFLOAT3 bonePos = pFbxModel_->GetBonePosition(boneName);
	XMVECTOR vBonePos = XMLoadFloat3(&bonePos)*GetAttachObject()->GetWorldMatrix();
	return StoreFloat3(vBonePos);
}

void ModelComponent::Draw()
{
	SetTransform();
	nowFrame_ += animSpeed_;
	if (nowFrame_ > endFrame_)
		nowFrame_ = startFrame_;
	pFbxModel_->Draw(transform_, shaderType_, (int)nowFrame_);
}

void ModelComponent::Release()
{
}

void ModelComponent::RayCast(ModelComponent* mComp, RayCastData& rayData)
{
	//対象のモデルのワールド行列の逆行列
	XMMATRIX matInv = XMMatrixInverse(nullptr, mComp->GetTransform().GetWorldMatrix());
	//レイの発射位置ベクトル
	XMVECTOR vStart = XMVectorSet(rayData.start.x,
								  rayData.start.y,
								  rayData.start.z, 0);
	//方向のベクトル
	XMVECTOR vPath = XMVectorSet(rayData.start.x + rayData.dir.x,
					   		     rayData.start.y + rayData.dir.y,
								 rayData.start.z + rayData.dir.z, 0);
	//ベクトルを逆行列で回転
	vStart = vStart * matInv;
	vPath = vPath * matInv;
	//startとdirに入れる
	rayData.start = StoreFloat3(vStart);
	rayData.dir = StoreFloat3(vPath-vStart);
	Transform trans = mComp->GetTransform();
	mComp->GetFbxModel()->RayCast(rayData, trans);
}


void ModelComponent::SetAnimFrame(int start, int end, float speed)
{
	nowFrame_ = (float)start;
	startFrame_ = start;
	endFrame_ = end;
	animSpeed_ = speed;
}