#pragma once
#include"MissionUI.h"
class ObjectSetter;
class ItemGetter;
class MissionOrder : public MissionUI
{
private:
	Text* pText_;
	Text* pCountText_;
	std::string count_;
	int hPict_;
	float ratio_;
	float alpha_;
	int EnemyMax_;
	int EnemyCount_;
	ObjectSetter* pSetter_;


public:
	MissionOrder(GameObject* parent);
	~MissionOrder();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void ThirdDraw() override;
	/// <summary>
	/// テキストをだんだん薄くする
	/// </summary>
	void EraseText();
	void Release() override;
};

