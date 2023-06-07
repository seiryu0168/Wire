#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/ResourceManager/Model.h"
#include"Engine/DirectX_11/Particle.h"
enum class ITEM_TYPE
{
	ITEM_NONE = 0,
	SPEED,
	SEARCH,
	ITEM_MAX,
};

class ItemBase : public GameObject
{
private:
	ITEM_TYPE itemType_;
	bool isCollision_;
	bool isAttached_;
	int iniTime_;
	std::string itemName_;
	int lifeTime_;
	GameObject* attacheObject_;
	float defaultParameter_;
	void SetIniTime(int time) { iniTime_ = time; }
public:
	Particle* pParticle_;
	ItemBase(GameObject* parent, std::string itemName);
	virtual ~ItemBase() {};
	//�A�C�e����K�p
	void AttachItem(GameObject* target = nullptr);
	//�A�C�e���^�C�v�ݒ�
	ITEM_TYPE GetItemType();
	//�A�C�e���̎����ݒ�
	int GetLifeTime();
	//�A�C�e�����擾�E���O�̃v���C���[�̃p�����[�^�擾
	float GetDefaultParameter() { return defaultParameter_; }
	//�A�C�e���̖��O�擾
	const std::string* GetItemName() { return &itemName_; };
	//�A�C�e���^�C�v�ݒ�
	void SetItemType(ITEM_TYPE type);
	//�A�C�e���̎����ݒ�
	void SetLifeTime(int time);
	//�A�C�e�����擾�E���O�̃v���C���[�̃p�����[�^�ݒ�
	void SetDefaultParameter(float param);
	//�A�C�e�����ݒ�
	void SetItemName(std::string name) { itemName_ = name; };
	//�A�C�e���������Ă���
	GameObject* GetAttacheObject() { return attacheObject_; }
	//�v���C���[�ɓ����������ǂ�����ݒ�
	void SetCollision();
	//���f�������ǂ���
	bool IsAttach();
	//�v���C���[�ɓ����������ǂ���
	bool IsCollision();
	//���������ɖ߂�
	void Reset() { lifeTime_ = iniTime_; }
	//�A�C�e���������Ă���p�[�e�B�N��
	virtual void PlayParticle(XMFLOAT3 pos) {};
	//�������炷
	void SubtractTime(int time=1);

};