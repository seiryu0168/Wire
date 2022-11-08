#include "Enemy_normal.h"
#include"Engine/Model.h"
#include"Pointer.h"
//コンストラクタ
Enemy_normal::Enemy_normal(GameObject* parent)
	:Enemy(parent,"Enemy_normal"),
	hModel_(-1),
	frontVec_(XMVectorSet(0,0,1,0)),
	matX_(XMMatrixIdentity()),
	matY_(XMMatrixIdentity()),
	vPosition_(XMVectorSet(0,0,0,0)),
	pPlayer_(nullptr)
{

}

//デストラクタ
Enemy_normal::~Enemy_normal()
{
	Release();
}

//初期化
void Enemy_normal::Initialize()
{
	hModel_ = Model::Load("Assets\\TestBox.fbx");
	pPlayer_ = (Player*)FindObject("Player");
	Instantiate<Pointer>(this);
}

//更新
void Enemy_normal::Update()
{
	vPosition_ =XMLoadFloat3(&transform_.position_);
	Pointer* pPointer = (Pointer*)FindChild("Pointer");
	XMFLOAT3 po;
	XMStoreFloat3(&po, vPosition_+frontVec_);
	pPointer->SetPointerPos(po);
	pPointer->SetDraw(true);
	
	XMVECTOR toPlayer;
	XMFLOAT3 aa = pPlayer_->GetPosition();
	toPlayer = XMVector3Normalize(XMLoadFloat3(&aa) - vPosition_);
	//XMVECTOR ss = XMVector3Dot(frontVec_, toPlayer);

	//float angle = acos(XMVectorGetZ(ss));

	if (IsVisible(frontVec_,XMLoadFloat3(&aa),0.5,-0.5))
	{
		EnemyMove(toPlayer);
	}
}

void Enemy_normal::FixedUpdate()
{

}
//描画
void Enemy_normal::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy_normal::EnemyMove(XMVECTOR toVec)
{
	vPosition_ = XMLoadFloat3(&transform_.position_);

	toVec = XMVector3Normalize(toVec) * 0.3f;
	vPosition_ += toVec;
	XMStoreFloat3(&transform_.position_, vPosition_);
}

bool Enemy_normal::IsVisible(XMVECTOR vFront, XMVECTOR vTarget, float angle1, float angle2)
{
	vFront = XMVector3Normalize(vFront);
	XMVECTOR toVector = vTarget - XMLoadFloat3(&transform_.position_);
	toVector = XMVector3Normalize(toVector);

	XMVECTOR dot = XMVector3Dot(vFront, toVector);
	float angle = acos(XMVectorGetX(dot));

	if (angle<angle1 && angle>angle2)
	{
		return true;
	}

	return false;
}

//開放
void Enemy_normal::Release()
{
}