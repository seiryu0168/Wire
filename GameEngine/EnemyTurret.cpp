#include "EnemyTurret.h"
#include"Stage1.h"
//#include"Engine/ResourceManager/Model.h"
#include"Bullet.h"
#include"HomingBullet.h"
#include"Pointer.h"
#include"Engine/Collider/SphereCollider.h"

namespace
{
	static const int SHOT_COUNT = 10;
	static const int RELOAD = 90;
}
void EnemyTurret::ChangeSatate(EnemyState<EnemyTurret>* state)
{
	if (state != pState_)
	{
		pState_ = state;
		pState_->Init(*this);
	}
}

//�R���X�g���N�^
EnemyTurret::EnemyTurret(GameObject* parent)
	:Enemy(parent,"EnemyTurret"),
	hModel_(-1),
	shotTime_(),
	rpm_(300),
	shotCount_(0),
	reLoadTime_(0)
{

}

//�f�X�g���N�^
EnemyTurret::~EnemyTurret()
{

}

//������
void EnemyTurret::Initialize()
{
	//�^�O�ݒ�
	SetTag("Enemy");
	//�v���C���[�̃|�C���^�[�擾
	SetPlayerPointer((Player*)FindObject("Player"));
	
	//�����蔻��ݒ�
	SphereCollider* pCollider = new SphereCollider(XMFLOAT3(0, 0, 0),3);
	AddCollider(pCollider);
	
	//���f���ǂݍ���
	hModel_ = ModelManager::Load("Assets\\EnemyTurret_Maya.fbx");
	assert(hModel_ >= 0);
	//���f���Z�b�g
	SethModel(hModel_);
	ModelManager::SetModelNum(hModel_);
	
	//�X�e�[�W�̃��f���擾
	int stageModelHandle = -1;
	stageModelHandle = ((Stage1*)FindObject("Stage1"))->GetModelHandle();
	
	//�����ʒu�ݒ�
	XMFLOAT3 initPos = transform_.position_;
	initPos = XMFLOAT3((float)((rand() % 1000)-500)/10.0f,0, (float)((rand() % 1000) - 500) / 10.0f);
	AdjustStartPos(initPos);
	initPos.y = 999.0f;
	//���C�L���X�g�Œn�ʂ����
	RayCastData ray;
	ray.start = initPos;
	ray.dir = { 0,-1,0 };
	ModelManager::RayCast(stageModelHandle, ray);
	if (ray.hit)
	{
		initPos.y = (999 - ray.dist)+2.0f;
	}
	transform_.position_ = initPos;
	//�{���X�e�[�g�ɐݒ�
	ChangeSatate(StateSearch::GetInstance());
}

//�X�V
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
	//�ˌ���������Ă邩
	if (isShot)
	{

		//shotTime��rpm_���g���Čv�Z�����l�ȏ�ɂȂ�����
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
//�`��
void EnemyTurret::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	if (IsLockOned(this))
		ModelManager::DrawOutLine(hModel_, { 1,0,0,1 });
	else if (GetPlayerPointer()->IsAim())
		ModelManager::DrawOutLine(hModel_, { 1,1,0,1 });
	else
		ModelManager::Draw(hModel_);
}

void EnemyTurret::Release()
{

}

void EnemyTurret::OnCollision(GameObject* pTarget)
{
	//���������I�u�W�F�N�g�̃^�O��Player��������
	if (pTarget->GetTag() == "Player")
	{
		//Player�̃X�e�[�^�X��ATTACK��������
		if (GetPlayerPointer()->GetSatatus() & ATC_ATTACK)
		{
			//HP��1���炷
			DecreaseLife(1);
			
			//Player�̃X�e�[�^�X��DEFAULT�ɖ߂�
			GetPlayerPointer()->SetStatus(ATC_DEFAULT);
			
			//HP��0�ȉ��ɂȂ�����
			if (GetLife() < 0)
			{
				//���W���������Ɉړ������Ă���E��
				Transform pos;
				pos.position_ = { 9999,9999,9999 };
				//���f���ԍ�������
				ModelManager::DeleteModelNum(hModel_);
				//�v���C���[�̔F�����X�g�������
				SetIsList(false);
				KillMe();
			}
		}
	}
}

void EnemyTurret::AdjustStartPos(XMFLOAT3& pos)
{
	//���W��y��0�ɂ��Ă���
	pos.y = 0;
	XMFLOAT3 playerPos = GetPlayerPointer()->GetPosition();
	playerPos.y = 0;
	//�v���C���[�Ƃ̋�����70�ȏゾ�����痣��
	if (VectorLength(pos - playerPos) <= 70)
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
