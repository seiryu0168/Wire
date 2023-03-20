#pragma once
#include<vector>
#include"Engine/GameObject/GameObject.h"
#include<string>

class MissionUI;
class PlayScreen : public GameObject
{
private:
	int hPict_[4];
	MissionUI* pUI;
public:
	PlayScreen(GameObject* parent);
	~PlayScreen();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void ThirdDraw() override;
	void Release() override;
	void SetDir(XMFLOAT2 dir);

};

