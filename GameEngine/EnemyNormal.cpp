#include "EnemyNormal.h"
#include"Engine/Model.h"
#include"Engine/BoxCollider.h"
#include"Pointer.h"
//コンストラクタ
EnemyNormal::EnemyNormal(GameObject* parent)
	:Enemy(parent, "EnemyNormal"),
	hModel_(-1)
{

}

//デストラクタ
EnemyNormal::~EnemyNormal()
{
	Release();
}

//初期化
void EnemyNormal::Initialize()
{
	hModel_ = Model::Load("Assets\\Enemy2.fbx");
	pPlayer_ = (Player*)FindObject("Player"); //確認用オブジェクト
	OBBCollider* pCollider = new OBBCollider(XMFLOAT3(1,1,1),false,false);
	AddCollider(pCollider);
	Model::SetModelNum(hModel_);

	transform_.position_.x = std::rand() % 100;
	transform_.position_.y = std::rand() % 100;
	transform_.position_.z = std::rand() % 100;
}

//更新
void EnemyNormal::Update()
{
	GetPositionVec() =XMLoadFloat3(&transform_.position_);    //今の座標をvPositionに入れる
	
	////////////////エネミーの処理/////////////
	XMFLOAT3 aa = GetPlayerPointer()->GetPosition();		//プレイヤーの座標取得
	SetToPlayerVec(XMLoadFloat3(&aa) - GetPositionVec());	//エネミーからプレイヤーに向かうベクトルを作成

	if (IsVisible(frontVec_,0.5,50.0f))
	{
		EnemyMove();

	}
}

void EnemyNormal::FixedUpdate()
{

}

//描画
void EnemyNormal::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void EnemyNormal::EnemyMove()
{
	SetPositionVec(XMLoadFloat3(&transform_.position_));//vPositionに今の座標を入れる
	XMVECTOR toTargetVec = XMLoadFloat3(&GetPlayerPointer()->GetPosition());
	XMVECTOR toVec = toTargetVec - GetPositionVec();
	toVec = XMVector3Normalize(toVec);							//引数の正規化
	float angle = XMVectorGetX(XMVector3Dot(GetFrontVec(), toVec)); //角度計算(ラジアン)
	transform_.rotate_.y = 1-angle;
	SetMatrixY(XMMatrixRotationY(transform_.rotate_.y));			//角度を回転行列に変換
	frontVec_ = XMVector3TransformCoord(toVec, matY_);			//前方向ベクトルを回転
	vPosition_ += toVec*0.3f;
	XMStoreFloat3(&transform_.position_, vPosition_);
}

//bool EnemyNormal::IsVisible(XMVECTOR vFront, float visibleAngle,float range)
//{
//
//	XMVECTOR toPlayer;
//	float rangeToPlayer;
//	rangeToPlayer = XMVectorGetX(XMVector3Length(toPlayerVec_));			//視界判定用の視界の長さ取得
//	toPlayer = XMVector3Normalize(toPlayerVec_);							//正規化
//
//	XMVECTOR dot = XMVector3Dot(vFront, toPlayerVec_);						//内積を計算
//	float angle = acos(min(XMVectorGetX(dot),1));						//角度計算(1以上にならないようmin関数つけた)
//	if (rangeToPlayer <= 2 * range)
//	{
//		pPlayer_->AddTargetList(this);
//		isTargetList_ = true;
//		if (angle<visibleAngle && angle>-visibleAngle && rangeToPlayer < range)
//		{
//			return true;
//		}
//	}
//	else
//	{
//		isTargetList_ = false;
//	}
//
//	return false;
//}
void EnemyNormal::Attack()
{

}

void EnemyNormal::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player")
	{
		if (pPlayer_->GetSatatus() & ATC_ATTACK)
		{
			life_--;
			pPlayer_->SetStatus(ATC_DEFAULT);
			if (life_ < 0)
			{
				Transform pos;
				pos.position_= { 9999,9999,9999 };
				Model::SetTransform(hModel_,pos);
				KillMe();
			}
		}
	}
}

//開放
void EnemyNormal::Release()
{
}