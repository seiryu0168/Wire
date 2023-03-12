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

//�R���X�g���N�^
EnemyNormal::EnemyNormal(GameObject* parent)
	:Enemy(parent, "EnemyNormal"),
	hModel_(-1),
	moveVec_(XMVectorZero()),
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
	SetPositionVec(XMLoadFloat3(&transform_.position_));	//vPosition�ɍ��̍��W������
	XMFLOAT3 playerPos = GetPlayerPointer()->GetPosition();
	XMVECTOR toTargetVec = XMLoadFloat3(&playerPos);
	XMVECTOR toVec = toTargetVec - GetPositionVec();
	toVec = XMVector3Normalize(toVec);						//�����̐��K��
	float angle = VectorDot(GetFrontVec(), toVec);			//�p�x�v�Z(���W�A��)
	SetMatrixY(XMMatrixRotationY(transform_.rotate_.y));	//�p�x����]�s��ɕϊ�
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
			DecreaseLife(1);				//���C�t��1���炷
			moveVec_ = -GetToPlayerVec();	//�m�b�N�o�b�N�p�̃x�N�g��
			knockBackTime_ = KNOCKBACKTIME; //�m�b�N�o�b�N�̎���(�t���[��)
			TurnToPlayer(GetToPlayerVec()); //�������������ɐU�����
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
	if (enemy.IsVisible(enemy.sight.angle_, enemy.sight.range_))
	{
		enemy.ChangeState(StateChase::GetInstance());
	}
}
