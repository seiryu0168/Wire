#include "EnemyNormal.h"
#include"Engine/ResourceManager/Model.h"
#include"Engine/Collider/SphereCollider.h"
#include"Pointer.h"
namespace
{
	static const int KNOCKBACKTIME = 30;
}

void EnemyNormal::ChangeState(EnemyState<EnemyNormal>* state)
{
	if (state != pState_)
	{
		pState_ = state;
		pState_->Init(*this);
	}
}

//コンストラクタ
EnemyNormal::EnemyNormal(GameObject* parent)
	:Enemy(parent, "EnemyNormal"),
	hModel_(-1),
	moveVec_(XMVectorZero()),
	knockBackTime_(0)
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
	hModel_ = ModelManager::Load("Assets\\EnemyBall.fbx");
	SethModel(hModel_);
	SphereCollider* pCollider = new SphereCollider(XMFLOAT3(0,0,0),3);
	AddCollider(pCollider);
	ModelManager::SetModelNum(hModel_);

	transform_.position_.x = (float)(std::rand() % 100);
	transform_.position_.y = (float)(std::rand() % 100);
	transform_.position_.z = (float)(std::rand() % 100);
	SetPlayerPointer((Player*)FindObject("Player"));
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
	if(IsLockOned(this))
		ModelManager::DrawOutLine(hModel_, {1,0,0,1});
	else if (GetPlayerPointer()->IsAim())
		ModelManager::DrawOutLine(hModel_, {1,1,0,1});
	else
		ModelManager::Draw(hModel_);
}

void EnemyNormal::EnemyMove()
{
	SetPositionVec(XMLoadFloat3(&transform_.position_));	//vPositionに今の座標を入れる
	XMFLOAT3 playerPos = GetPlayerPointer()->GetPosition();
	XMVECTOR toTargetVec = XMLoadFloat3(&playerPos);
	XMVECTOR toVec = toTargetVec - GetPositionVec();
	toVec = XMVector3Normalize(toVec);						//引数の正規化
	float angle = VectorDot(GetFrontVec(), toVec);			//角度計算(ラジアン)
	SetMatrixY(XMMatrixRotationY(transform_.rotate_.y));	//角度を回転行列に変換
	toVec *= 0.6f;
	toVec+= GetPositionVec();
	moveVec_ *= (float)knockBackTime_/(float)KNOCKBACKTIME;
	//moveVec_ += moveVec_;
	SetPositionVec(toVec);
	XMStoreFloat3(&transform_.position_, toVec+moveVec_);
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
			DecreaseLife(1);				//ライフを1減らす
			moveVec_ = -GetToPlayerVec();	//ノックバック用のベクトル
			knockBackTime_ = KNOCKBACKTIME; //ノックバックの時間(フレーム)
			TurnToPlayer(GetToPlayerVec()); //当たった方向に振り向く
			GetPlayerPointer()->SetStatus(ATC_DEFAULT);
			if (GetLife() < 0)
			{
				Transform pos;
				pos.position_= { 9999,9999,9999 };
				ModelManager::DeleteModelNum(hModel_);
				SetIsList(false);
				KillMe();
			}
		}
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
	if (enemy.IsVisible(enemy.sight.angle_, enemy.sight.range_))
	{
		enemy.ChangeState(StateChase::GetInstance());
	}
}
