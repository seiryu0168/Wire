#include "Particle.h"
#include"Engine/Model.h"
Particle::Particle(GameObject* parent)
	:GameObject(parent,"Particle"),
	hModel_(-1)
{

}

//デストラクタ
Particle::~Particle()
{

}

//初期化
void Particle::Initialize()
{
}

//更新
void Particle::Update()
{

}

void Particle::FixedUpdate()
{

}
//描画
void Particle::Draw()
{

}

//開放
void Particle::Release()
{

}