#include "EnemyNormal.h"
#include"Stage1.h"
//#include"ModelComponent.h"
#include"Engine/Collider/SphereCollider.h"
#include"Pointer.h"
namespace
{
	static const int KNOCKBACKTIME = 30;
	static const float MAX_TO_PLAYERDIST = 70.0f;
	static const XMFLOAT4 COLOR_RED = { 1,0,0,1 };
	static const XMFLOAT4 COLOR_YELLOW = { 1,1,0,1 };
}


//コンストラクタ
EnemyNormal::EnemyNormal(GameObject* parent)
	:Enemy(parent, "EnemyNormal"),
	hModel_(-1),
	knockBackVec_(XMVectorZero()),
	knockBackTime_(0),
	moveSpeed_(0.6f)
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
	SetTag("Enemy");
	SetPlayerPointer((Player*)FindObject("Player"));
	hModel_ = ModelManager::Load("Assets\\EnemyBall.fbx");
	SethModel(hModel_);
	SphereCollider* pCollider = new SphereCollider(XMFLOAT3(0,0,0),3);
	AddCollider(pCollider);
	ModelManager::SetModelNum(hModel_);

	XMFLOAT3 startPos = transform_.position_;// XMFLOAT3((float)((std::rand() % 3000) - 1500) / 10.0f, 0, (float)((std::rand() % 3000) - 1500) / 10.0f);
	
	AdjustStartPos(startPos);
	startPos.y = 999.0f;
	RayCastData ray;
	ray.start = startPos;
	ray.dir = XMFLOAT3(0, -1, 0);
	int stageModelHandle = ((Stage1*)FindObject("Stage1"))->GetModelHandle();
	ModelManager::RayCast(stageModelHandle,ray);
	transform_.position_ = StoreFloat3(ray.hitPos);
	transform_.position_.y += 10;
	ChangeState(StateSearch::GetInstance());
}

//更新
void EnemyNormal::Update()
{
	SetPositionVec(XMLoadFloat3(&transform_.position_));
	if (knockBackTime_ > 0)
	{
		knockBackTime_--;
	}
	pState_->Update(*this);
}

void EnemyNormal::FixedUpdate()
{

}

//描画
void EnemyNormal::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	//ロックオンされてたら
	if (IsLockOned(this))
		ModelManager::DrawOutLine(hModel_, COLOR_RED);
	//ロックオンされてないが、プレイヤーがエイムモードになっている
	else if (GetPlayerPointer()->IsAim())
		ModelManager::DrawOutLine(hModel_, COLOR_YELLOW);
	//何もなし
	else
		ModelManager::Draw(hModel_);
}

void EnemyNormal::EnemyMove()
{
	//vPositionに今の座標を入れる
	SetPositionVec(XMLoadFloat3(&transform_.position_));	
	//プレイヤーへのベクトル
	XMVECTOR toVec = GetPlayerPointer()->GetPosition() - transform_.position_;
	//引数の正規化
	toVec = XMVector3Normalize(toVec);	
	//角度計算(ラジアン)
	float angle = VectorDot(GetFrontVec(), toVec);
	//角度を回転行列に変換
	SetMatrixY(XMMatrixRotationY(transform_.rotate_.y));
	//ベクトル調整
	toVec *= moveSpeed_;
	toVec+= GetPositionVec();
	
	//ノックバックベクトル計算
	knockBackVec_ *= (float)knockBackTime_/(float)KNOCKBACKTIME;
	//ベクトル設定
	SetPositionVec(toVec);
	XMStoreFloat3(&transform_.position_, toVec+knockBackVec_);
}

void EnemyNormal::Attack()
{
	EnemyMove();
}

void EnemyNormal::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetTag() == "Player")
	{

		if (GetPlayerPointer()->GetSatatus() & ATC_ATTACK)
		{
			//HPを1減らす
			DecreaseLife(1);				
			
			//ノックバック用のベクトル
			knockBackVec_ = -GetToPlayerVec();
			//ノックバックの時間(フレーム)
			knockBackTime_ = KNOCKBACKTIME;
			//当たった方向に振り向く
			TurnToPlayer(GetToPlayerVec()); 
			//PlayerのステータスをDEFAULTに戻す
			GetPlayerPointer()->SetStatus(ATC_DEFAULT);
			//HPが0になったら
			if (GetLife() < 0)
			{
				//座標を遠い所にする
				Transform pos;
				pos.position_= { 9999,9999,9999 };
				//モデル番号を消す
				ModelManager::DeleteModelNum(hModel_);
				//プレイヤーの認識リストから消す
				SetIsList(false);
				KillMe();
			}
		}
	}
}

void EnemyNormal::AdjustStartPos(XMFLOAT3& pos)
{
	//座標をy＝0にしておく
	pos.y = 0;
	XMFLOAT3 playerPos = GetPlayerPointer()->GetPosition();
	playerPos.y = 0;
	//プレイヤーとの距離が70以上だったら離す
	if (VectorLength(pos - playerPos) <= MAX_TO_PLAYERDIST)
	{
		pos = StoreFloat3(XMLoadFloat3(&pos) + (pos - playerPos));
	}
}

//開放
void EnemyNormal::Release()
{
}

void EnemyNormal::StateChase::Init(EnemyNormal& enemy)
{

}

void EnemyNormal::StateChase::Update(EnemyNormal& enemy)
{
	enemy.EnemyMove();
	if (!enemy.IsVisible(enemy.sight.angle_, enemy.sight.range_))
	{
		enemy.ChangeState(StateSearch::GetInstance());
	}
}

EnemyNormal::StateSearch::~StateSearch()
{
}

void EnemyNormal::StateSearch::Init(EnemyNormal& enemy)
{
}

void EnemyNormal::StateSearch::Update(EnemyNormal& enemy)
{
	//視界内にプレイヤーを捉えたら
	if (enemy.IsVisible(enemy.sight.angle_, enemy.sight.range_))
	{
		enemy.ChangeState(StateChase::GetInstance());
	}
}

void EnemyNormal::ChangeState(EnemyState<EnemyNormal>* state)
{
	if (state != pState_)
	{
		pState_ = state;
		pState_->Init(*this);
	}
}