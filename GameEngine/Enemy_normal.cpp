#include "Enemy_normal.h"
#include"Engine/Model.h"
#include"Pointer.h"
//コンストラクタ
Enemy_normal::Enemy_normal(GameObject* parent)
	:Enemy(parent, "Enemy_normal"),
	hModel_(-1),
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
Enemy_normal::~Enemy_normal()
{
	Release();
}

//初期化
void Enemy_normal::Initialize()
{
	hModel_ = Model::Load("Assets\\TestBox.fbx");
	pPlayer_ = (Player*)FindObject("Player"); //確認用オブジェクト
	Instantiate<Pointer>(this);
}

//更新
void Enemy_normal::Update()
{
	vPosition_ =XMLoadFloat3(&transform_.position_);    //今の座標をvPositionに入れる


	////////////////確認用オブジェクト表示////////////
	Pointer* pPointer = (Pointer*)FindChild("Pointer");
	XMFLOAT3 po;
	XMStoreFloat3(&po, vPosition_+frontVec_);
	pPointer->SetPointerPos(po);
	pPointer->SetDraw(true);
	
	////////////////エネミーの処理/////////////
	XMVECTOR toPlayer;
	XMFLOAT3 aa = pPlayer_->GetPosition();		//プレイヤーの座標取得
	toPlayer = XMLoadFloat3(&aa) - vPosition_;	//エネミーからプレイヤーに向かうベクトルを作成

	if (IsVisible(frontVec_,XMLoadFloat3(&aa),0.5,50.0f)/*&&visibleFlag_*/)
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
	
	vPosition_ = XMLoadFloat3(&transform_.position_);			//vPositionに今の座標を入れる
	toVec = XMVector3Normalize(toVec);							//引数の正規化
	float angle = XMVectorGetX(XMVector3Dot(frontVec_, toVec)); //角度計算(ラジアン)
	matY_ = XMMatrixRotationY(1-angle);							//角度を回転行列に変換
	frontVec_ = XMVector3TransformCoord(toVec, matY_);			//前方向ベクトルを回転
	
	vPosition_ += toVec*0.3f;
	XMStoreFloat3(&transform_.position_, vPosition_);
}

bool Enemy_normal::IsVisible(XMVECTOR vFront, XMVECTOR vTarget, float visibleAngle,float range)
{
	vFront = XMVector3Normalize(vFront);								//正規化
	XMVECTOR toVector = vTarget - XMLoadFloat3(&transform_.position_);	//目標に向かうベクトル
	float rangeToPlayer;
	rangeToPlayer = XMVectorGetX(XMVector3Length(toVector));			//視界判定用の視界の長さ取得
	toVector = XMVector3Normalize(toVector);							//正規化

	XMVECTOR dot = XMVector3Dot(vFront, toVector);						//内積を計算
	float angle = acos(min(XMVectorGetX(dot),1));						//角度計算(1以上にならないようmin関数つけた)


	if (angle<visibleAngle && angle>-visibleAngle&&rangeToPlayer<range)
	{
		return true;
	}

	return false;
}

//開放
void Enemy_normal::Release()
{
}