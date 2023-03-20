#include "EnemyNormal.h"
#include"Stage1.h"
#include"Engine/Collider/SphereCollider.h"
#include"Pointer.h"
namespace
{
	static const int KNOCKBACKTIME = 30;
}


//�R���X�g���N�^
EnemyNormal::EnemyNormal(GameObject* parent)
	:Enemy(parent, "EnemyNormal"),
	hModel_(-1),
	knockBackVec_(XMVectorZero()),
	knockBackTime_(0)
{

}

//�f�X�g���N�^
EnemyNormal::~EnemyNormal()
{
	Release();
}

//������
void EnemyNormal::Initialize()
{
	SetTag("Enemy");
	SetPlayerPointer((Player*)FindObject("Player"));
	hModel_ = ModelManager::Load("Assets\\EnemyBall.fbx");
	SethModel(hModel_);
	SphereCollider* pCollider = new SphereCollider(XMFLOAT3(0,0,0),3);
	AddCollider(pCollider);
	ModelManager::SetModelNum(hModel_);

	XMFLOAT3 startPos = XMFLOAT3((float)((std::rand() % 3000) - 1500) / 10.0f, 0, (float)((std::rand() % 3000) - 1500) / 10.0f);
	
	AdjustStartPos(startPos);
	startPos.y = 999.0f;
	RayCastData ray;
	ray.start = startPos;
	ray.dir = XMFLOAT3(0, -1, 0);
	ModelManager::RayCast(ray);
	transform_.position_ = StoreFloat3(ray.hitPos);
	transform_.position_.y += 10;
	ChangeState(StateSearch::GetInstance());
}

//�X�V
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

//�`��
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
	//vPosition�ɍ��̍��W������
	SetPositionVec(XMLoadFloat3(&transform_.position_));	
	//�v���C���[�ւ̃x�N�g��
	XMVECTOR toVec = GetPlayerPointer()->GetPosition() - transform_.position_;
	//�����̐��K��
	toVec = XMVector3Normalize(toVec);	
	//�p�x�v�Z(���W�A��)
	float angle = VectorDot(GetFrontVec(), toVec);
	//�p�x����]�s��ɕϊ�
	SetMatrixY(XMMatrixRotationY(transform_.rotate_.y));
	//�x�N�g������
	toVec *= 0.6f;
	toVec+= GetPositionVec();
	
	//�m�b�N�o�b�N�x�N�g���v�Z
	knockBackVec_ *= (float)knockBackTime_/(float)KNOCKBACKTIME;
	//�x�N�g���ݒ�
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
			//HP��1���炷
			DecreaseLife(1);				
			
			//�m�b�N�o�b�N�p�̃x�N�g��
			knockBackVec_ = -GetToPlayerVec();
			//�m�b�N�o�b�N�̎���(�t���[��)
			knockBackTime_ = KNOCKBACKTIME;
			//�������������ɐU�����
			TurnToPlayer(GetToPlayerVec()); 
			//Player�̃X�e�[�^�X��DEFAULT�ɖ߂�
			GetPlayerPointer()->SetStatus(ATC_DEFAULT);
			//HP��0�ɂȂ�����
			if (GetLife() < 0)
			{
				//���W���������ɂ���
				Transform pos;
				pos.position_= { 9999,9999,9999 };
				//���f���ԍ�������
				ModelManager::DeleteModelNum(hModel_);
				//�v���C���[�̔F�����X�g�������
				SetIsList(false);
				KillMe();
			}
		}
	}
}

void EnemyNormal::AdjustStartPos(XMFLOAT3& pos)
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

//�J��
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
	//���E���Ƀv���C���[�𑨂�����
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