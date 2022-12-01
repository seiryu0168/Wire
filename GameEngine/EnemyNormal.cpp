#include "EnemyNormal.h"
#include"Engine/Model.h"
#include"Engine/BoxCollider.h"
#include"Pointer.h"
//コンストラクタ
EnemyNormal::EnemyNormal(GameObject* parent)
	:Enemy(parent, "EnemyNormal"),
	hModel_(-1),
	life_(5),
	toPlayerVec_(XMVectorSet(0,0,0,0)),
	frontVec_(XMVectorSet(0, 0, 1, 0)),
	upVec_(XMVectorSet(0,1,0,0)),
	matX_(XMMatrixIdentity()),
	matY_(XMMatrixIdentity()),
	visibleFlag_(false),
	vPosition_(XMVectorSet(0,0,0,0)),
	pPlayer_(nullptr)
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
}

//更新
void EnemyNormal::Update()
{
	vPosition_ =XMLoadFloat3(&transform_.position_);    //今の座標をvPositionに入れる

	////////////////確認用オブジェクト表示////////////
	/*Pointer* pPointer = (Pointer*)FindChild("Pointer");
	XMFLOAT3 po;
	XMStoreFloat3(&po, vPosition_+frontVec_);
	pPointer->SetPointerPos(po);*/
	//pPointer->SetDraw(true);
	
	////////////////エネミーの処理/////////////
	XMVECTOR toPlayer;
	XMFLOAT3 aa = pPlayer_->GetPosition();		//プレイヤーの座標取得
	toPlayer = XMLoadFloat3(&aa) - vPosition_;	//エネミーからプレイヤーに向かうベクトルを作成

	if (IsVisible(frontVec_,XMLoadFloat3(&aa),0.5,50.0f))
	{
		EnemyMove(toPlayer);
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

void EnemyNormal::EnemyMove(XMVECTOR toVec)
{
	vPosition_ = XMLoadFloat3(&transform_.position_);			//vPositionに今の座標を入れる
	toVec = XMVector3Normalize(toVec);							//引数の正規化
	float angle = XMVectorGetX(XMVector3Dot(frontVec_, toVec)); //角度計算(ラジアン)
	matY_ = XMMatrixRotationY(1-angle);							//角度を回転行列に変換
	frontVec_ = XMVector3TransformCoord(toVec, matY_);			//前方向ベクトルを回転
	
	vPosition_ += toVec*0.3f;
	XMStoreFloat3(&transform_.position_, vPosition_);
}

bool EnemyNormal::IsVisible(XMVECTOR vFront, XMVECTOR vTarget, float visibleAngle,float range)
{
	vFront = XMVector3Normalize(vFront);								//正規化
	 toPlayerVec_ = vTarget - XMLoadFloat3(&transform_.position_);	//目標に向かうベクトル
	float rangeToPlayer;
	rangeToPlayer = XMVectorGetX(XMVector3Length(toPlayerVec_));			//視界判定用の視界の長さ取得
	toPlayerVec_ = XMVector3Normalize(toPlayerVec_);							//正規化

	XMVECTOR dot = XMVector3Dot(vFront, toPlayerVec_);						//内積を計算
	float angle = acos(min(XMVectorGetX(dot),1));						//角度計算(1以上にならないようmin関数つけた)

	if (angle<visibleAngle && angle>-visibleAngle && rangeToPlayer < range)
	{
		return true;
	}

	return false;
}

XMVECTOR EnemyNormal::GetToPlayerVector()
{
	return toPlayerVec_;
}

void EnemyNormal::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player")
	{
		if (pPlayer_->GetSatatus() & ATC_ATTACK)
		{
			EnemyMove(pPlayer_->GetPlayerMove());

		}
		
	}
}

//開放
void EnemyNormal::Release()
{
}