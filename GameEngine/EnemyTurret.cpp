#include "EnemyTurret.h"
#include"Stage1.h"
#include"ModelComponent.h"
//#include"Engine/ResourceManager/Model.h"
#include"Bullet.h"
#include"HomingBullet.h"
#include"Pointer.h"
#include"Engine/Collider/SphereCollider.h"

namespace
{
	static const int SHOT_COUNT = 10;
	static const int RELOAD = 90;
	static const float MAX_TO_PLAYERDIST = 60.0f;
	static const float COLLISION_SIZE = 3.0f;
	static const XMFLOAT4 COLOR_RED = { 1,0,0,1 };
	static const XMFLOAT4 COLOR_YELLOW = { 1,1,0,1 };
	static const float INIT_POS_Y = 2.0f;
	static const float INIT_OFFSET = 999.0f;
	static const int MAX_RANDOM = 500;
	static const std::string TURRET_MODEL = "Assets\\Model\\EnemyTurret_Maya.fbx";
}
void EnemyTurret::ChangeSatate(EnemyState<EnemyTurret>* state)
{
	if (state != pState_)
	{
		pState_ = state;
		pState_->Init(*this);
	}
}

//コンストラクタ
EnemyTurret::EnemyTurret(GameObject* parent)
	:Enemy(parent,"EnemyTurret"),
	hModel_(-1),
	shotTime_(),
	rpm_(300),
	shotCount_(0),
	reLoadTime_(0),
	pState_(nullptr)
{

}

//デストラクタ
EnemyTurret::~EnemyTurret()
{

}

//初期化
void EnemyTurret::Initialize()
{
	//タグ設定
	SetTag("Enemy");
	//プレイヤーのポインター取得
	SetPlayerPointer((Player*)FindObject("Player"));
	
	//当たり判定設定
	SphereCollider* pCollider = new SphereCollider(XMFLOAT3(0, 0, 0), COLLISION_SIZE);
	AddCollider(pCollider);
	
	//モデル読み込み
	hModel_ = ModelManager::Load(TURRET_MODEL);
	assert(hModel_ >= 0);

	//モデルセット
	SethModel(hModel_);
	ModelManager::SetModelNum(hModel_);
	
	//ステージのモデル取得
	int stageModelHandle = -1;
	stageModelHandle = ((Stage*)FindObject("Stage1"))->GetModelHandle();
	
	//初期位置設定
	XMFLOAT3 initPos = transform_.position_;
	//initPos = XMFLOAT3(CalcRandomPosition(MAX_RANDOM), 0, CalcRandomPosition(MAX_RANDOM));
	AdjustStartPos(initPos);
	initPos.y = INIT_OFFSET;
	//レイキャストで地面を特定
	RayCastData ray;
	ray.start = initPos;
	ray.dir = { 0,-1,0 };
	ModelManager::RayCast(stageModelHandle,ray);
	if (ray.hit)
	{
		initPos.y = (INIT_OFFSET - ray.dist)+INIT_POS_Y;
	}
	transform_.position_ = initPos;
	//捜索ステートに設定
	ChangeSatate(StateSearch::GetInstance());
}

//更新
void EnemyTurret::Update()
{
	SetPositionVec(XMLoadFloat3(&transform_.position_));
	pState_->Update(*this);
}

void EnemyTurret::Attack()
{
}

void EnemyTurret::Shot(bool isShot)
{
		shotTime_++;
	//射撃が許可されてるか
	if (isShot)
	{

		//shotTimeがrpm_を使って計算した値以上になったら
		if ((3600.0f / (float)rpm_) <= (float)shotTime_)
		{
			XMVECTOR shotDir = XMVector3Normalize(GetToPlayerVec());
			Bullet* pBullet = Instantiate<Bullet>(this);
			pBullet->SetDir(shotDir);
			shotCount_++;
			shotTime_ = 0;
		}
	}

}

void EnemyTurret::FixedUpdate()
{

}
//描画
void EnemyTurret::Draw()
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

float EnemyTurret::CalcRandomPosition(int radius)
{
	return ((rand() % (radius * 2)) - radius) / 10.0f;
}

void EnemyTurret::Release()
{

}

void EnemyTurret::OnCollision(GameObject* pTarget)
{
	//当たったオブジェクトのタグがPlayerだったら
	if (pTarget->GetTag() == "Player")
	{
		//PlayerのステータスがATTACKだったら
		if (GetPlayerPointer()->GetSatatus() & ATC_ATTACK)
		{
			//HPを1減らす
			DecreaseLife(1);
			
			//PlayerのステータスをDEFAULTに戻す
			GetPlayerPointer()->SetStatus(ATC_DEFAULT);
			
			//HPが0以下になったら
			if (GetLife() < 0)
			{
				//座標を遠い所に移動させてから殺す
				Transform pos;
				pos.position_ = { 9999,9999,9999 };
				//モデル番号も消す
				ModelManager::DeleteModelNum(hModel_);
				//プレイヤーの認識リストから消す
				SetIsList(false);
				KillMe();
			}
		}
	}
}

void EnemyTurret::AdjustStartPos(XMFLOAT3& pos)
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

void EnemyTurret::StateChase::Init(EnemyTurret& enemy)
{
	enemy.shotTime_ = 0;
	enemy.reLoadTime_ = 0;
}

void EnemyTurret::StateChase::Update(EnemyTurret& enemy)
{
	if (enemy.shotCount_ % 10 == 0 && perShot_)
	{
		enemy.reLoadTime_ = RELOAD;
		perShot_ = false;
	}
	else
	{
		enemy.reLoadTime_--;
		enemy.reLoadTime_ = max(enemy.reLoadTime_, 0);
	}

	if(enemy.reLoadTime_==0)
	{
		perShot_ = true;
	}
		enemy.Shot(perShot_);


	if (!enemy.IsVisible(enemy.sight.angle_, enemy.sight.range_))
	{
		enemy.ChangeSatate(StateSearch::GetInstance());
	}
}

void EnemyTurret::StateSearch::Init(EnemyTurret& enemy)
{
}

void EnemyTurret::StateSearch::Update(EnemyTurret& enemy)
{
	if (enemy.IsVisible(enemy.sight.angle_, enemy.sight.range_))
	{
		enemy.ChangeSatate(StateChase::GetInstance());
	}
}
