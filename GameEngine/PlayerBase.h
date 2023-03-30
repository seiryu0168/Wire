#pragma once
#include"Engine/GameObject/GameObject.h"

class Particle;
class PlayerBase : public GameObject
{
private:
	enum class SPARKPOS : int
	{
		FRONT_LEFT=0,
		FRONT_RIGHT,
		BACK_LEFT,
		BACK_RIGHT,
		MAX
	};
	int hModel_;
	int hAudio_;
	Particle* pParticle_;
	XMFLOAT3 prevPos_;
	XMVECTOR moveVec_;
	XMVECTOR vSparkPos_[4];
public:
	PlayerBase(GameObject* parent);
	~PlayerBase();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void Spark();
};

