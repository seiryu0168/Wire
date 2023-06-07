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
	//アイテムを適用
	void AttachItem(GameObject* target = nullptr);
	//アイテムタイプ設定
	ITEM_TYPE GetItemType();
	//アイテムの寿命設定
	int GetLifeTime();
	//アイテムを取得拾う前のプレイヤーのパラメータ取得
	float GetDefaultParameter() { return defaultParameter_; }
	//アイテムの名前取得
	const std::string* GetItemName() { return &itemName_; };
	//アイテムタイプ設定
	void SetItemType(ITEM_TYPE type);
	//アイテムの寿命設定
	void SetLifeTime(int time);
	//アイテムを取得拾う前のプレイヤーのパラメータ設定
	void SetDefaultParameter(float param);
	//アイテム名設定
	void SetItemName(std::string name) { itemName_ = name; };
	//アイテムを持っている
	GameObject* GetAttacheObject() { return attacheObject_; }
	//プレイヤーに当たったかどうかを設定
	void SetCollision();
	//反映したかどうか
	bool IsAttach();
	//プレイヤーに当たったかどうか
	bool IsCollision();
	//寿命を元に戻す
	void Reset() { lifeTime_ = iniTime_; }
	//アイテムが発しているパーティクル
	virtual void PlayParticle(XMFLOAT3 pos) {};
	//寿命減らす
	void SubtractTime(int time=1);

};