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
	//ModelManager�ɓ��ꃂ�f���̗L�����m�F���āA���f�����������ꍇ��
	//���̃��f���̃|�C���^���i�[
	//�Ȃ������ꍇ�͓ǂݍ���
	pFbxModel_ = ModelManager::LoadModel(fileName);
	hModel_ = ModelManager::AddMComponentList(this);
}

void ModelComponent::SetTransform(const Transform* transform)
{
	//transform��nullptr��������
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
	//�R���|�[�l���g���A�N�e�B�u�Ȃ�
	if (IsActive())
	{
		Draw();
	}
}

XMFLOAT3 ModelComponent::GetBonPosition(std::string boneName)
{
	//�{�[���̈ʒu���擾�A�ό`���ĕԂ�
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
	//�Ώۂ̃��f���̃��[���h�s��̋t�s��
	XMMATRIX matInv = XMMatrixInverse(nullptr, mComp->GetTransform().GetWorldMatrix());
	//���C�̔��ˈʒu�x�N�g��
	XMVECTOR vStart = XMVectorSet(rayData.start.x,
								  rayData.start.y,
								  rayData.start.z, 0);
	//�����̃x�N�g��
	XMVECTOR vPath = XMVectorSet(rayData.start.x + rayData.dir.x,
					   		     rayData.start.y + rayData.dir.y,
								 rayData.start.z + rayData.dir.z, 0);
	//�x�N�g�����t�s��ŉ�]
	vStart = vStart * matInv;
	vPath = vPath * matInv;
	//start��dir�ɓ����
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